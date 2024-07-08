#include "verifier.hpp"
#include "../frame/frame.hpp"
#include "../frame/thread.hpp"
#include "../loader/errReport.hpp"
#include "../builtin/primitive.hpp"
#include "../ast.hpp"
#include "../frame/starter.hpp"
#include "../loader/worker/worker.inl"

namespace nm {

    NM_DEF_ME(verifier)

    namespace {
        str primitives[] = {
            new nInt(),
            new nFlt(),
            new nStr(),
            new nByte(),
            new nChar(),
            new nBool(),
        };

        nbool checkEvalType(const node& eval) {
            if(nul(eval)) return false;
            for(str e : primitives)
                if(eval.isSub(*e))
                    return true;

            return false;
        }

        static ncnt _stepN = 0;
    }

#define _GUARD(msg) \
    if(isFlag(GUARD)) do { \
        NM_I("'%s' %s@%s: " msg, i, ttype<typeTrait<decltype(me)>::Org>::get(), platformAPI::toAddrId(&me)); \
        _stepN = 0; \
    } while(0)

#define _STEP(msg, ...) NM_I("'%s' %s@%s: step#%s --> " msg, i, \
    ttype<typeTrait<decltype(me)>::Org>::get(), platformAPI::toAddrId(&me), ++_stepN, ## __VA_ARGS__)

    // verification:
    void me::onLeave(visitInfo i, node& me) {
        _GUARD("onLeave()");

        _STEP("no same variable=%s", me.subs().len());
        if(me.isSub<frame>()) return;

        for(auto e=me.subs().begin(); e ;++e) {
            auto matches = me.subAll<baseObj>(e.getKey());
            if(matches.len() > 1)
                return posError(errCode::DUP_VAR, *e, e.getKey().c_str());
        }
    }

    void me::onLeave(visitInfo i, asExpr& me) {
        _GUARD("onVisit()");

        _STEP("_me & _as aren't null");
        if(nul(me.getMe())) return posError(errCode::LHS_IS_NULL, me);
        if(nul(me.getAs())) return posError(errCode::RHS_IS_NULL, me);
        if(me.getAs().isSub<nVoid>()) return posError(errCode::VOID_NOT_CAST, me);

        _STEP("checks that me can cast to 'as'");
        if(!me.getMe().is(me.getAs()))
            return posError(errCode::CAST_NOT_AVAILABLE, me, me.getMe().getType().getName().c_str(),
                    me.getAs().getType().getName().c_str());

        _STEP("rhs shouldn't be expression");
        if(!me.getAs().isImpli<node>())
            return posError(errCode::CAST_TO_UNKNOWN, me);
    }

    void me::onLeave(visitInfo i, assignExpr& me) {
        _GUARD("onVisit()");

        _STEP("set evalType");
        str leftEval = me.getLeft().getEval();
        if(!leftEval) return posError(errCode::LHS_IS_NULL, me);
        const ntype& ltype = leftEval->getType();
        if(nul(ltype)) return posError(errCode::LHS_IS_NULL, me);
        str rightEval = me.getRight().getEval();
        if(!rightEval) return posError(errCode::RHS_IS_NULL, me);
        const ntype& rtype = rightEval->getType();
        if(nul(rtype)) return posError(errCode::RHS_IS_NULL, me);
        if(rtype.isSub<retStateExpr>()) return posError(errCode::CANT_ASSIGN_RET, me);
        if(!rtype.isImpli(ltype))
            return posError(errCode::TYPE_NOT_COMPATIBLE, me, rtype.getName().c_str(), ltype.getName()
                    .c_str());

        // verify rvalue and lvalue:
        //  first of all:
        //      in namulang there is no lvalue concept. to involve lvalue concept,
        //      I need to bring new concept or feature like 'const' but I don't want to.
        //      because the direction of this language is a toy lang. DON'T be serious.
        //      but I can't leave something which is going to operate like a lvalue by user.
        //      so I'll emulate the way how lvalue works.
        //
        //  when you assign something:
        //      into left identifier (= lhs), left one should be lvalue. and there are 3 case
        //      of lvalue you can find:
        //
        //      1. getExpr:
        //          assigning identifier registered into some scope is always valid
        //          operation.
        //          e.g. A.B = 5
        //
        //      2. RunExpr:
        //          running some method of object could come in chain of assignment exprs.
        //              e.g. A.B().C = 5
        //          however it shouldn't be the last expression to process.
        //              e.g. A.B() = 5 // invalid
        //
        //      3. ElementExpr:
        //          getting element by index also valid expression of assignment expr.
        //              e.g. A.B[C] = 5
        //          when verifier meets ElementExpr, it doesn't need to deep search any longer to
        //          verify invalidness of lvalue.
        //
        //      it looks complicated to verify this at glance but it's not.
        //      the reason is, structure of expression has been inside out:
        //          e.g. A.B.NAME = 5
        //
        //      the structure of above sample will be,
        //          AssignExpr(getExpr(getExpr(A, B), NAME), nInt(5))
        //      so, only I need to check is, lhs of AssignExpr is kind of getExpr() or not.
        //      I can care about that the last expression is valid.
        _STEP("checks rvalue");
        const node& lhs = me.getLeft();
        if(!lhs.isSub<getExpr>()/* TODO: && !lhs.isSub<ElementExpr>()*/)
            return posError(errCode::ASSIGN_TO_RVALUE, me, me.getRight().getType().getName().c_str(),
                    lhs.getType().getName().c_str());
    }

    void me::onLeave(visitInfo i, blockExpr& me) {
        _GUARD("onLeave()");

        _STEP("last stmt should match to ret type");
        const narr& stmts = me.getStmts();
        if(nul(stmts) || stmts.len() <= 0) return; // will be catched to another verification.

        func& parent = i.parent ? i.parent->cast<func>() : nulOf<func>();
        if(!nul(parent))
            _verifyMgdFuncImplicitReturn(parent);
    }

    void me::_onLeave(const visitInfo& i, const loopExpr& me) {
        _GUARD("onLeave()");

        str eval = me.getEval(); // it's okay forExpr not to have 'eval'.
        if(eval) {
            _STEP("eval Value check: eval[%s] is an array?", eval);
            if(!eval->isSub<retStateExpr>() && !eval->isSub<arr>())
                return posError(errCode::LOOP_NO_RET_ARR, me);
        }

        me.getBlock().outFrame();
        _recentLoops.pop_back();
    }

    void me::onLeave(visitInfo i, defVarExpr& me) {
        _GUARD("onLeave()");

        _STEP("is definable?");
        const node& rhs = me.getRight();
        if(nul(rhs))
            return posError(errCode::CANT_DEF_VAR, me, me.getName().c_str(), "null");

        _STEP("to define a void type property isn't allowed.");
        str eval = rhs.getEval();
        if(!eval) return posError(errCode::RHS_IS_NULL, me);
        if(eval->isSub<nVoid>())
            return posError(errCode::VOID_CANT_DEFINED, me);

        obj& cast = eval->cast<obj>();
        if(!nul(cast)) {
            const obj& org = cast.getOrigin();
            if(!org.isPreEvaluated())
                return posError(errCode::TYPE_IS_NOT_PRE_EVALUATED, me);
        }

        _STEP("does rhs[%s] have 'ret' in its blockStmt?", eval);
        if(eval->isSub<retStateExpr>())
            return posError(errCode::CANT_ASSIGN_RET, me);

        _STEP("check whether make a void container.");
        const narr& beans = eval->getType().getBeans();
        for(const node& bean : beans)
            if(bean.isSub<nVoid>())
                return posError(errCode::NO_VOID_CONTAINER, me);

        std::string name = me.getName();
        if(name == "") return posError(errCode::HAS_NO_NAME, me);
        _STEP("is %s definable?", name);
        const ntype& t = eval->getType();
        const nchar* typeName = nul(t) ? "null" : t.getName().c_str();
        if(nul(t))
            posError(errCode::CANT_DEF_VAR, me, name.c_str(), typeName);
        if(eval->isSub<nVoid>()) return posError(errCode::VOID_CANT_DEFINED, me);

        node& to = me.getTo();
        _STEP("is 'to'[%s] valid", to);
        // only if to is 'frame', I need to make property when verify:
        //  local variables are required to verify further statements. but it's okay. it'll been
        //  removed after blockExpr::outFrame().
        //
        //  however, if you make a variable on defBlock, that matters and affects rest of real
        //  execution of codes. because they still leaves after verification because they aren't on
        //  local scope.
        //  but don't worry. these kind of properties have to be evaluated and instantiated on
        //  the pre-evaluation step.
        if(!nul(to)) return;

        frame& fr = thread::get()._getNowFrame();
        _STEP("duplication of variable with name[%s]", name);
        if(fr.mySubs()->has(name))
            return posError(errCode::ALREADY_DEFINED_VAR, me, name.c_str(), typeName);

        _STEP("ok. defining local temporary var... %s %s", name, typeName);
        fr.addLocal(name, *new mockNode(*eval));
    }

    void me::onLeave(visitInfo i, defAssignExpr& me) {
        _GUARD("onVisit()");

        const node& rhs = me.getRight();
        str eval = !nul(rhs) ? rhs.getEval() : str();
        if(!eval) return posError(errCode::RHS_IS_NULL, me);
        if(!eval->isComplete())
            return posError(errCode::ACCESS_TO_INCOMPLETE, me);

        onLeave(i, (defAssignExpr::super&) me);
    }

    void me::onLeave(visitInfo i, defSeqExpr& me) {
        _GUARD("onVisit()");

        _STEP("check lhs & rhs");
        if(nul(me.getStart())) return posError(errCode::LHS_IS_NULL, me);
        if(nul(me.getEnd())) return posError(errCode::RHS_IS_NULL, me);

        _STEP("lhs & rhs is sort of Int?");
        if(!me.getStart().isImpli<nInt>()) return posError(errCode::SEQ_SHOULD_INT_COMPATIBLE, me);
        if(!me.getEnd().isImpli<nInt>()) return posError(errCode::SEQ_SHOULD_INT_COMPATIBLE, me);
    }

    void me::onLeave(visitInfo i, defArrayExpr& me) {
        _GUARD("onVisit()");

        _STEP("check all elements");
        const node& type = me.getArrayType();
        if(nul(type)) return posError(errCode::ELEM_TYPE_DEDUCED_NULL, me);
        if(type.isSuper<obj>())
            return posError(errCode::ELEM_TYPE_DEDUCED_WRONG, me, type.getType().getName().c_str());
        if(type.isSub<nVoid>())
            return posError(errCode::ELEM_TYPE_NOT_VOID, me);
    }

    void me::onLeave(visitInfo i, FBOExpr& me) {
        _GUARD("onVisit()");

        _STEP("lhs & rhs should bind something.");
        const node& lhs = me.getLeft();
        const node& rhs = me.getRight();
        if(nul(lhs)) return posError(errCode::LHS_IS_NULL, me);
        if(nul(rhs)) return posError(errCode::RHS_IS_NULL, me);

        _STEP("finding eval of l(r)hs.");
        str lEval = lhs.getEval();
        str rEval = rhs.getEval();
        if(!lEval) return posError(errCode::LHS_IS_NULL, me);
        if(!rEval) return posError(errCode::RHS_IS_NULL, me);

        if(!checkEvalType(*lEval)) return posError(errCode::LHS_IS_NOT_ARITH, me, lEval->getType().getName().c_str());
        if(!checkEvalType(*rEval)) return posError(errCode::RHS_IS_NOT_ARITH, me, rEval->getType().getName().c_str());

        if(nul(lEval->deduce(*rEval)))
            return posError(errCode::IMPLICIT_CAST_NOT_AVAILABLE, me,
                    lEval->getType().getName().c_str(), rEval->getType().getName().c_str());

        auto r = me.getRule();
        if((lEval->isSub<nStr>() || rEval->isSub<nStr>())) {
            switch(r) {
                case FBOExpr::AND: case FBOExpr::OR: case FBOExpr::SUB: case FBOExpr::DIV:
                case FBOExpr::MOD: case FBOExpr::BITWISE_AND: case FBOExpr::BITWISE_XOR:
                case FBOExpr::BITWISE_OR: case FBOExpr::LSHIFT: case FBOExpr::RSHIFT:
                    return posError(errCode::STRING_IS_NOT_PROPER_TO_OP, me);

                default:;
            }
        }
    }

    void me::onLeave(visitInfo i, FUOExpr& me) {
        _GUARD("onLeave()");

        _STEP("string isn't proper to any FUO operator");
        str eval = me.getEval();
        if(eval && eval->isImpli<nStr>())
            return posError(errCode::STRING_IS_NOT_PROPER_TO_OP, me);
    }

    void me::onLeave(visitInfo i, getExpr& me) {
        _GUARD("onLeave()");

        // TODO: I have to check that the evalType has what matched to given _params.
        // Until then, I rather use as() func and it makes slow emmersively.
        _STEP("isRunnable: %s.%s", me, me.getName());
        if(!me.getEval()) return posError(errCode::WHAT_IS_THIS_IDENTIFIER, me, me.getName().c_str());
        auto matches = me._get(true);
        if(matches.isEmpty()) {
            const node& from = me.getMe();
            return posError(errCode::CANT_ACCESS, me, me._name.c_str(), from.getType().getName().c_str());
        }
        node& got = matches.get();
        if(nul(got)) {
            // TODO: leave logs for all ambigious candidates as err.
            return posError(errCode::AMBIGIOUS_ACCESS, me, i.name.c_str());
        }
        _STEP("isRunnable: got=%s, me=%s", got, me.getType());

        str asedMe = me.getMe().getEval();
        _STEP("accesses to incomplete 'me[%s]' object?", asedMe);
        if(asedMe && !asedMe->isComplete())
            return posError(errCode::ACCESS_TO_INCOMPLETE, me);
    }

    void me::onLeave(visitInfo i, retExpr& me) {
        _GUARD("onVisit()");

        _STEP("should be at last stmt");
        if(i.index != i.len-1)
            return posError(errCode::RET_AT_MIDDLE_OF_BLOCK, me);

        _STEP("checks evalType of func is matched to me");
        const baseFunc& f = thread::get().getNowFrame().getFunc();
        if(nul(f)) return posError(errCode::NO_FUNC_INFO, me);

        str myRet = me.getRet().getEval();
        if(!myRet) return posError(errCode::EXPR_EVAL_NULL, me);

        str funRet = f.getRet()->as<node>();
        _STEP("checks return[%s] == func[%s]", myRet, funRet);

        if(!myRet->isSub<err>() && !myRet->isImpli(*funRet))
            return posError(errCode::RET_TYPE_NOT_MATCH, me, myRet->getType().getName().c_str(),
                            funRet->getType().getName().c_str());
    }

    void me::onLeave(visitInfo i, runExpr& me) {
        _GUARD("onVisit()");

        _STEP("is it possible to run?");
        if(nul(me.getMe())) return posError(errCode::DONT_KNOW_ME, me);

        str ased = me.getMe().getEval();
        if(!ased) return;

        args& a = me.getArgs();
        a.setMe(*ased);

        node& anySub = me.getSubj();
        if(nul(anySub)) return posError(errCode::FUNC_NOT_EXIST, me);
        _STEP("anySub[%s]", anySub);

        str derivedSub = anySub.getEval();
        if(!derivedSub) return posError(errCode::CANT_ACCESS, me, ased->getType().getName().c_str(), "sub-node");

        _STEP("derivedSub[%s]", derivedSub);
        if(!derivedSub->canRun(me.getArgs())) {
            const baseFunc& derivedCast = derivedSub->cast<baseFunc>();
            std::string params = nul(derivedCast) ? "ctor" : _asStr(derivedCast.getParams());
            return posError(errCode::OBJ_WRONG_ARGS, me, i.name.c_str(), me.getArgs().asStr().c_str(), params.c_str());
        }

        a.setMe(nulOf<baseObj>());
    }

    void me::onTraverse(runExpr& me, node& subject) {
        node& meSubj = me.getMe();
        if(nul(meSubj)) return;

        str ased = meSubj.getEval();
        if(!ased) return;

        getExpr& cast = subject.cast<getExpr>();
        if(!nul(cast))
            cast.setMe(*ased);
    }

    std::string me::_asStr(const params& ps) {
        std::string ret;
        nbool first = true;
        for(const param& p : ps) {
            ret += (first ? "" : ",") + p.getOrigin().getType().getName();
            first = false;
        }

        return ret;
    }

    nbool me::onVisit(visitInfo i, func& me) {
        _GUARD("onVisit()");

        onLeave(i, (func::super&) me);

        obj& meObj = thread::get()._getNowFrame().getMe().cast<obj>(); // TODO: same to 'thread::get().getNowFrame().getMe().cast<obj>();'
        if(nul(meObj)) return posError(errCode::FUNC_REDIRECTED_OBJ, me), true;

        _STEP("check func duplication");
        const nbicontainer& top = meObj.getShares().getContainer();
        ncnt len = me.getParams().len();
        const node& errFound = top.get([&](const std::string& key, const node& val) {
            if(key != i.name) return false;
            if(&val == &me) return false;

            const baseFunc& cast = val.cast<baseFunc>();
            if(nul(cast))
                // this has same name on shares, but it's not func! it's not valid.
                // this could be an origin obj.
                return true;

            const params& castPs = cast.getParams();
            if(castPs.len() != len) return false;

            for(int n=0; n < castPs.len() ;n++) {
                str lhs = castPs[n].getOrigin().getEval();
                str rhs = me.getParams()[n].getOrigin().getEval();
                if(lhs->getType() != rhs->getType())
                    return false;
            }

            return true;
        });
        if(!nul(errFound))
            posError(errCode::ALREADY_DEFINED_FUNC, me, i.name.c_str());

        //  obj or property shouldn't have same name to any func.
        _STEP("check func has same name to field");
        if(!nul(meObj.getOwns().get(i.name)))
            posError(errCode::ALREADY_DEFINED_IDENTIFIER, me, i.name.c_str());

        _STEP("main func return type should be int or void");
        if(i.name == starter::MAIN) {
            str ret = me.getRet();
            if(!ret->isSub<nInt>() && !ret->isSub<nVoid>())
                posError(errCode::MAIN_FUNC_RET_TYPE, me);
        }

        _STEP("retType exists and stmts exist one at least");
        str retType = me.getRet();
        if(!retType) return posError(errCode::NO_RET_TYPE, me), true;
        if(!retType->isSub(ttype<node>::get()))
            return posError(errCode::WRONG_RET_TYPE, me, retType->getType().getName().c_str()), true;

        blockExpr& blk = (blockExpr&) me.getBlock();
        if(nul(blk) || blk.getStmts().len() <= 0) {
            if(i.name == starter::MAIN)
                posError(errCode::MAIN_SHOULD_HAVE_STMTS, blk);
            /*TODO: uncomment after implement abstract:
            if(!func.isAbstract() && !retType->isSub<nVoid>())
                error(...)
            */
            return true;
        }

        _STEP("'break' or 'next' can't be used to last stmt");
        const node& lastStmt = *blk.getStmts().last();
        if(lastStmt.isSub<retStateExpr>() && !lastStmt.isSub<retExpr>())
            return posError(errCode::FUNC_SHOULD_RETURN_SOMETHING, lastStmt), true;

        _STEP("func[%s]: %s iterateBlock[%s]", i, me, me._blk->subs().len());

        // sequence of adding frame matters:
        //  object scope was added at 'onVisit(visitInfo, baseObj&)
        //  parameters of func is second:
        scope* s = new scope();
        for(const auto& p : me.getParams()) {
            if(p.getOrigin().isSub<nVoid>()) {
                posError(errCode::PARAM_NOT_VOID, me, p.getName().c_str());
                continue;
            }
            s->add(p.getName(), *(node*) p.getOrigin().getEval()->clone());
        }

        //  function's subs are third:
        me.inFrame(*s);
        return true;
    }

    void me::_verifyMgdFuncImplicitReturn(func& me) {
        str ret = me.getRet()->as<node>();
        const type& retType = ret->getType();
        const node& lastStmt = *me.getBlock().getStmts().last();

        if(retType == ttype<nVoid>::get())
            return NM_I("func: implicit return won't verify when retType is void."), void();

        str eval = me.getBlock().getEval();
        if(!eval) return posError(NO_RET_TYPE, lastStmt);

        const ntype& lastType = eval->getType(); // to get type of expr, always uses evalType.
        if(nul(lastType)) return posError(NO_RET_TYPE, lastStmt);

        NM_I("func: last stmt[%s] should matches to return type[%s]", eval, retType);

        if(eval->isSub<retStateExpr>())
            // @see retExpr::getEval() for more info.
            return NM_I("func: skip verification when lastStmt is retStateExpr."), void();

        if(!lastType.isSub<err>() && !lastType.isImpli(retType))
            return posError(errCode::RET_TYPE_NOT_MATCH, lastStmt,
                            lastType.getName().c_str(), retType.getName().c_str());
    }

    void me::_prepare() {
        super::_prepare();
        _recentLoops.clear();
    }

    void me::onLeave(visitInfo i, func& me) {
        _GUARD("onLeave()");

        me.outFrame(scope());
    }

    nbool me::onVisit(visitInfo i, baseObj& me) {
        _GUARD("onVisit()");

        me.inFrame();

        frame& fr = thread::get()._getNowFrame();
        _STEP("%s push me[%s] len=%s", fr.getMe(), (void*) &fr.getMe(), me.subs().len());

        _STEP("iterate all subs and checks void type variable");
        for(const node& elem : me.subs())
            if(elem.isSub<nVoid>())
                posError(errCode::VOID_CANT_DEFINED, elem);

        onLeave(i, (baseObj::super&) me);
        return true;
    }

    void me::onLeave(visitInfo i, baseObj& me) {
        _GUARD("onLeave()");
        me.outFrame();
    }

    nbool me::onVisit(visitInfo i, genericObj& me) {
        _GUARD("onVisit()");

        _STEP("cache check");
        for(auto e : me._cache)
            if(nul(e.second))
                posError(errCode::MAKE_GENERIC_FAIL, me, e.first.c_str());
        return true;
    }

    void me::onLeave(visitInfo i, genericObj& me) {
        _GUARD("onLeave()");

        // DO NOTHING, BUT LEAVE THIS FUNC:
        //  if I don't have this func, getGenericExpr::super (=baseObj)'s one will be called.
        //  and me pointer will be erased too inside the func.
    }

    nbool me::onVisit(visitInfo i, forExpr& me) {
        _GUARD("onVisit()");

        str container = me._container;
        str conAsed = container->getEval();
        if(!conAsed)
            return posError(errCode::CONTAINER_IS_NULL, me), true;
        str elemType = conAsed->run("getElemType");
        if(!elemType)
            return posError(errCode::ELEM_TYPE_IS_NULL, me), true;

        const std::string& name = me.getLocalName();
        _STEP("define iterator '%s %s'", elemType, name);

        me.getBlock().inFrame();
        thread::get()._getNowFrame().addLocal(name, *((node*) elemType->clone()));
        _recentLoops.push_back(&me);
        return true;
    }

    void me::onLeave(visitInfo i, forExpr& me) {
        _GUARD("onLeave()");

        _onLeave(i, me);
    }

    nbool me::onVisit(visitInfo i, whileExpr& me) {
        _GUARD("onVisit()");

        me.getBlock().inFrame();
        _recentLoops.push_back(&me);
        return true;
    }

    void me::onLeave(visitInfo i, whileExpr& me) {
        _GUARD("onLeave()");

        _onLeave(i, me);
    }

    void me::onLeave(visitInfo i, breakExpr& me) {
        _GUARD("onVisit()");

        _STEP("declared outside of loop?");
        if(_recentLoops.size() <= 0) return posError(errCode::BREAK_OUTSIDE_OF_LOOP, me);
    }

    void me::onLeave(visitInfo i, nextExpr& me) {
        _GUARD("onVisit()");

        _STEP("declared outside of loop?");
        if(_recentLoops.size() <= 0) return posError(errCode::NEXT_OUTSIDE_OF_LOOP, me);
    }

    nbool me::onVisit(visitInfo i, ifExpr& me) {
        _GUARD("onVisit()");
        me.getThen().inFrame();
        return true;
    }

    void me::onLeave(visitInfo i, ifExpr& me) {
        _GUARD("onLeave()");
        blockExpr().outFrame(); // it doesn't matter getting blockExpr from 'me'.
                                // because conceptually, blockExpr::outFrame() is just like static func.
    }

    void me::onTraverse(ifExpr& me, blockExpr& blk) {
        if(nul(blk)) return;

        me.getThen().outFrame();
        blk.inFrame();
    }
}
