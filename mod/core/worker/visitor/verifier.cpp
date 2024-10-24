#include "verifier.hpp"

#include "../../ast.hpp"
#include "../../builtin/primitive.hpp"
#include "../../frame/frame.hpp"
#include "../../frame/starter.hpp"
#include "../../frame/thread.hpp"
#include "../../builtin/err/errReport.hpp"
#include "../worker.inl"
#include "common/macro.hpp"

namespace nm {

    NM_DEF_ME(verifier)

    namespace {
        static std::vector<str> _getPrimitives() {
            static std::vector<str> inner;
            if(inner.size() <= 0) {
                inner.push_back(new nInt());
                inner.push_back(new nFlt());
                inner.push_back(new nStr());
                inner.push_back(new nByte());
                inner.push_back(new nChar());
                inner.push_back(new nBool());
            };
            return inner;
        }

        nbool checkEvalType(const node& eval) {
            if(nul(eval)) return false;
            for(str e: _getPrimitives())
                if(eval.isSub(*e)) return true;

            return false;
        }

        static ncnt _stepN = 0;
    }

#define thenErr_1(code) nothing(), posError(errCode::code)
#define thenErr_2(code, a1) nothing(), posError(errCode::code, a1)
#define thenErr_3(code, a1, a2) nothing(), posError(errCode::code, a1, a2)
#define thenErr_4(code, a1, a2, a3) nothing(), posError(errCode::code, a1, a2, a3)
#define thenErr_5(code, a1, a2, a3, a4) nothing(), posError(errCode::code, a1, a2, a3, a4)
#define thenErr(...) NM_OVERLOAD(thenErr, __VA_ARGS__)

#define thenWarn_2(code, a1) nothing(), posWarn(errCode::code, a1)
#define thenWarn_3(code, a1, a2) nothing(), posWarn(errCode::code, a1, a2)
#define thenWarn_4(code, a1, a2, a3) nothing(), posWarn(errCode::code, a1, a2, a3)
#define thenWarn_5(code, a1, a2, a3, a4) nothing(), posWarn(errCode::code, a1, a2, a3, a4)
#define thenWarn(...) NM_OVERLOAD(thenWarn, __VA_ARGS__)

#define _GUARD(msg)                                                                \
    if(isFlag(GUARD)) do {                                                         \
            NM_I("'%s' %s@%s: " msg, i, me.getType(), platformAPI::toAddrId(&me)); \
            _stepN = 0;                                                            \
    } while(0)

#define _STEP(msg, ...)                                                                 \
    NM_I("'%s' %s@%s: step#%d --> " msg, i, ttype<typeTrait<decltype(me)>::Org>::get(), \
        platformAPI::toAddrId(&me), ++_stepN, ##__VA_ARGS__)

    // verification:
    void me::onLeave(const visitInfo& i, node& me) {
        _GUARD("onLeave()");

        _STEP("no same variable=%d", me.subs().len());
        if(me.isSub<frame>()) return;

        for(auto e = me.subs().begin(); e; ++e) {
            auto matches = me.subAll<baseObj>(e.getKey());

            NM_WHEN(matches.len() > 1).thenErr(DUP_VAR, *e, e.getKey());
        }
    }

    void me::onLeave(const visitInfo& i, asExpr& me) {
        _GUARD("onLeave()");

        _STEP("_me & _as aren't null");
        NM_WHENNUL(me.getMe()).thenErr(LHS_IS_NUL, me);
        NM_WHENNUL(me.getAs()).thenErr(RHS_IS_NUL, me);
        NM_WHEN(me.getAs().isSub<nVoid>()).thenErr(VOID_NOT_CAST, me);

        _STEP("checks that me can cast to 'as'");
        NM_WHEN(!me.getMe().is(me.getAs())).thenErr(CAST_NOT_AVAILABLE, me, me.getMe(), me.getAs());

        _STEP("rhs shouldn't be expression");
        NM_WHEN(!me.getAs().isImpli<node>()).thenErr(CAST_TO_UNKNOWN, me);
    }

    void me::onLeave(const visitInfo& i, isExpr& me) {
        _GUARD("onLeave()");

        _STEP("_me & _to aren't null");
        NM_WHENNUL(me.getMe()).thenErr(LHS_IS_NUL, me);
        NM_WHENNUL(me.getTo()).thenErr(RHS_IS_NUL, me);

        _STEP("checks that me can cast to 'as'");
        NM_WHEN(!me.getMe().is(me.getTo())).thenErr(CAST_NOT_AVAILABLE, me, me.getMe(), me.getTo());

        _STEP("rhs shouldn't be expression");
        NM_WHEN(!me.getTo().isImpli<node>()).thenErr(CAST_TO_UNKNOWN, me);
    }

    void me::onLeave(const visitInfo& i, assignExpr& me) {
        _GUARD("onVisit()");

        _STEP("set evalType");

        const ntype& ltype = me THEN(getLeft()) THEN(getEval()) THEN(getType());
        NM_WHENNUL(ltype).thenErr(LHS_IS_NUL, me);
        const ntype& rtype = me THEN(getRight()) THEN(getEval()) THEN(getType());
        NM_WHENNUL(rtype).thenErr(RHS_IS_NUL, me);
        NM_WHEN(rtype.isSub<retStateExpr>()).thenErr(CANT_ASSIGN_RET, me);
        NM_WHEN(!rtype.isImpli(ltype)).thenErr(TYPE_NOT_COMPATIBLE, me, rtype, ltype);

        // verify rvalue and lvalue:
        //  first of all:
        //      in namulang there is no lvalue concept. to involve lvalue concept,
        //      I need to bring new concept or feature like 'const' but I don't want to.
        //      because the direction of this language is a toy lang. DON'T be serious.
        //      but I can't leave something which is going to operate like a lvalue by user.
        //      so I'll emulate the way how lvalue works.
        //
        //  NM_WHEN you assign something:
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
        //          NM_WHEN verifier meets ElementExpr, it doesn't need to deep search any longer to
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
        NM_WHEN(!lhs.isSub<getExpr>() /*TODO: && !lhs.isSub<ElementExpr>()*/)
            .thenErr(ASSIGN_TO_RVALUE, me, me.getRight(), lhs);

        const getExpr& leftCast = me.getLeft().cast<getExpr>();
        NM_WHEN(util::checkTypeAttr(leftCast.getName()) == ATTR_CONST)
            .thenErr(ASSIGN_TO_CONST, me, leftCast.getName());
    }

    nbool me::onVisit(const visitInfo& i, blockExpr& me) {
        _GUARD("onVisit()");

        me.inFrame();
        return true;
    }

    void me::onLeave(const visitInfo& i, blockExpr& me) {
        _GUARD("onLeave()");

        const narr& stmts = me.getStmts();
        if(nul(stmts) || stmts.len() <= 0) return; // will be catched to another verification.

        me.setEval(*me.getEval());
        me.outFrame();
    }

    void me::_onLeave(const visitInfo& i, const loopExpr& me) {
        _GUARD("onLeave()");
        _recentLoops.pop_back();

        str eval = me.getEval(); // it's okay forExpr not to have 'eval'.
        if(eval) {
            _STEP("eval Value check: eval[%s] is an array?", eval);
            NM_WHEN(!eval->isSub<retStateExpr>() && !eval->isSub<arr>())
                .thenErr(LOOP_NO_RET_ARR, me);
        }
    }

    void me::onLeave(const visitInfo& i, defVarExpr& me) {
        _GUARD("onLeave()");

        _STEP("modifier not allowed for local variables.");
        const auto& mod = me.getNewModifier();
        NM_WHENNUL(mod).thenErr(MODIFIER_NOT_FOUND, me, me.getName());
        NM_WHEN(!mod.isPublic()).thenErr(PROTECTED_NOT_ALLOWED_FOR_LOCAL, me, me.getName());
        NM_WHEN(mod.isExplicitOverride()).thenErr(OVERRIDE_NOT_ALLOWED_FOR_LOCAL, me, me.getName());

        _STEP("to define a void type property isn't allowed.");
        str eval = me.getRight() THEN(getEval());
        NM_WHEN(!eval).thenErr(RHS_IS_NUL, me);
        NM_WHEN(eval->isSub<nVoid>()).thenErr(VOID_CANT_DEFINED, me);

        obj& cast = eval->cast<obj>();
        if(!nul(cast)) {
            const baseObj& org = cast.getOrigin();
            NM_WHEN(org.getState() < PARSED).thenErr(TYPE_IS_NOT_PRE_EVALUATED, me, me.getName());
        }

        _STEP("does rhs[%s] have 'ret' in its blockStmt?", eval);
        NM_WHEN(eval->isSub<retStateExpr>()).thenErr(CANT_ASSIGN_RET, me);

        _STEP("check whether make a void container.");
        const auto& ps = eval->getType().getParams();
        for(const auto& p: ps)
            NM_WHEN(p.getOrigin().isSub<nVoid>()).thenErr(NO_VOID_CONTAINER, me);

        std::string name = me.getName();
        NM_WHEN(name == "").thenErr(HAS_NO_NAME, me);

        _STEP("is %s definable?", name);
        const ntype& t = eval->getType();
        if(nul(t)) posError(errCode::CANT_DEF_VAR, me, name.c_str(), "null");
        NM_WHEN(eval->isSub<nVoid>()).thenErr(VOID_CANT_DEFINED, me);

        node& to = me.getTo();
        _STEP("is 'to'[%s] valid", to);
        // only if to is 'frame', I need to make property NM_WHEN verify:
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
        NM_WHEN(fr.mySubs()->in(name)).thenErr(ALREADY_DEFINED_VAR, me, name, t);

        _STEP("ok. defining local temporary var... %s %s", name, t);
        fr.addLocal(name, *new mockNode(*eval));
    }

    void me::onLeave(const visitInfo& i, defPropExpr& me) {
        _GUARD("onLeave()");

        _STEP("whether the 'type' object has a ctor without any paramters?");
        str eval = me THEN(getRight()) THEN(getEval());
        NM_WHEN(!eval).thenErr(RHS_IS_NUL, me);
        NM_WHENNUL(eval->sub(baseObj::CTOR_NAME, args{})).thenErr(DONT_HAVE_CTOR, me, eval);

        onLeave(i, (defPropExpr::super&) me);
    }

    void me::onLeave(const visitInfo& i, defAssignExpr& me) {
        _GUARD("onVisit()");

        _STEP("check rhs");
        str eval = me THEN(getRight()) THEN(getEval());
        NM_WHEN(!eval).thenErr(RHS_IS_NUL, me);
        NM_WHEN(!eval->isComplete()).thenErr(ACCESS_TO_INCOMPLETE, me);

        str explicitType = me THEN(getExplicitType());
        if(explicitType) {
            _STEP("check explicit type whether it's valid");
            str type = me THEN(getEval());
            NM_WHENNUL(type).thenErr(EXPLICIT_TYPE_SHOULDNT_BE_NULL, me);
            NM_WHEN(type->isSub<nVoid>()).thenErr(NO_VOID_VARIABLE, me);
        }

        onLeave(i, (defAssignExpr::super&) me);
    }

    void me::onLeave(const visitInfo& i, defSeqExpr& me) {
        _GUARD("onVisit()");

        _STEP("check lhs & rhs");
        auto& start = me.getStart();
        NM_WHENNUL(start).thenErr(LHS_IS_NUL, me);
        auto& end = me.getEnd();
        NM_WHENNUL(end).thenErr(RHS_IS_NUL, me);

        _STEP("lhs & rhs is sort of Int?");
        NM_WHEN(!start.isImpli<nInt>()).thenErr(SEQ_SHOULD_INT_COMPATIBLE, me);
        NM_WHEN(!end.isImpli<nInt>()).thenErr(SEQ_SHOULD_INT_COMPATIBLE, me);
    }

    void me::onLeave(const visitInfo& i, defArrayExpr& me) {
        _GUARD("onVisit()");

        _STEP("check all elements");
        const node& type = me.getArrayType();
        NM_WHENNUL(type).thenErr(ELEM_TYPE_DEDUCED_NUL, me);
        NM_WHEN(type.isSuper<obj>()).thenErr(ELEM_TYPE_DEDUCED_WRONG, me, type);
        NM_WHEN(type.isSub<nVoid>()).thenErr(ELEM_TYPE_NOT_VOID, me);

        _STEP("check arr has exactly 1 type parameter.");
        const auto& ps = me.getOrigin() THEN(getType()) THEN(getParams());
        NM_WHENNUL(ps).thenErr(ELEM_TYPE_IS_NUL, me);
        NM_WHEN(ps.len() != 1).thenErr(ARR_DOESNT_HAVE_TYPE_PARAM, me);
    }

    void me::onLeave(const visitInfo& i, FBOExpr& me) {
        _GUARD("onVisit()");

        _STEP("finding eval of l(r)hs.");
        str lEval = me THEN(getLeft()) THEN(getEval());
        str rEval = me THEN(getRight()) THEN(getEval());
        NM_WHEN(!lEval).thenErr(LHS_IS_NUL, me);
        NM_WHEN(!rEval).thenErr(RHS_IS_NUL, me);

        NM_WHEN(!checkEvalType(*lEval)).thenErr(LHS_IS_NOT_ARITH, me, lEval);
        NM_WHEN(!checkEvalType(*rEval)).thenErr(RHS_IS_NOT_ARITH, me, rEval);

        NM_WHENNUL(lEval->deduce(*rEval)).thenErr(IMPLICIT_CAST_NOT_AVAILABLE, me, lEval, rEval);

        auto r = me.getRule();
        if((lEval->isSub<nStr>() || rEval->isSub<nStr>())) {
            switch(r) {
                case FBOExpr::AND:
                case FBOExpr::OR:
                case FBOExpr::SUB:
                case FBOExpr::DIV:
                case FBOExpr::MOD:
                case FBOExpr::BITWISE_AND:
                case FBOExpr::BITWISE_XOR:
                case FBOExpr::BITWISE_OR:
                case FBOExpr::LSHIFT:
                case FBOExpr::RSHIFT: return posError(errCode::STRING_IS_NOT_PROPER_TO_OP, me);

                default:;
            }
        }
    }

    void me::onLeave(const visitInfo& i, FUOExpr& me) {
        _GUARD("onLeave()");

        _STEP("string isn't proper to any FUO operator");
        str eval = me.getEval();
        NM_WHEN(eval && eval->isImpli<nStr>()).thenErr(STRING_IS_NOT_PROPER_TO_OP, me);
    }

    void me::onLeave(const visitInfo& i, getExpr& me) {
        _GUARD("onLeave()");

        // TODO: I have to check that the evalType has what matched to given _params.
        // Until then, I rather use as() func and it makes slow emmersively.
        _STEP("isRunnable: %s.%s", me, me.getName());
        NM_WHEN(!me.getEval()).thenErr(WHAT_IS_THIS_IDENTIFIER, me, me.getName());
        str match = me._get(true);
        if(!match) {
            const node& from = me.getMe();
            return posError(errCode::CANT_ACCESS, me, me._name.c_str(),
                from.getType().getName().c_str());
        }
        // TODO: leave logs for all ambigious candidates as err.
        NM_WHENNUL(*match).thenErr(AMBIGIOUS_ACCESS, me, i);

        _STEP("isRunnable: got=%s, me=%s", match, me.getType());

        str asedMe = me.getMe().getEval();
        _STEP("accesses to incomplete 'me[%s]' object?", asedMe);
        NM_WHEN(asedMe && !asedMe->isComplete()).thenErr(ACCESS_TO_INCOMPLETE, me);

        _STEP("check modifier.");
        const auto& mod = match->getModifier();
        NM_WHENNUL(mod).thenErr(MODIFIER_NOT_FOUND, me, me.getName());
        if(!mod.isPublic()) { // we only need to run verify routine when there is protected
                              // modifier.
            baseObj& castedMe = asedMe->cast<baseObj>();
            if(!nul(castedMe)) { // if getExpr's castedMe is not derived one of baseObj, it's frame.
                const node& currentMe = thread::get().getNowFrame().getMe();
                NM_WHEN(!castedMe.isSuper(currentMe))
                    .thenErr(CANT_ACCESS_TO_PROTECTED_VARIABLE, me, me.getName());
            }
        }
    }

    void me::onLeave(const visitInfo& i, retExpr& me) {
        _GUARD("onLeave()");

        _STEP("should be at last stmt");
        NM_WHEN(i.index != i.len - 1).thenErr(RET_AT_MIDDLE_OF_BLOCK, me);

        _STEP("checks evalType of func is matched to me");
        const baseFunc& f = thread::get().getNowFrame().getFunc();
        NM_WHENNUL(f).thenErr(NO_FUNC_INFO, me);

        str myRet = me.getRet().getEval();
        NM_WHEN(!myRet).thenErr(EXPR_EVAL_NUL, me);

        const node& funRet = f.getRet();
        _STEP("checks return[%s] == func[%s]", myRet, funRet);

        NM_WHEN(!myRet->isSub<baseErr>() && !myRet->isImpli(funRet))
            .thenErr(RET_TYPE_NOT_MATCH, me, myRet, funRet);
    }

    void me::onLeave(const visitInfo& i, runExpr& me) {
        _GUARD("onLeave()");

        _STEP("is it possible to run?");
        NM_WHENNUL(me.getMe()).thenErr(DONT_KNOW_ME, me);

        str ased = me.getMe().getEval() orRet;
        args& a = me.getArgs();
        a.setMe(*ased);

        node& anySub = me.getSubj();
        NM_WHENNUL(anySub).thenErr(FUNC_NOT_EXIST, me);
        _STEP("anySub[%s]", anySub);

        str derivedSub = anySub.getEval();
        NM_WHEN(!derivedSub).thenErr(CANT_ACCESS, me, ased, "sub-node");

        _STEP("derivedSub[%s]", derivedSub);
        if(!derivedSub->canRun(me.getArgs())) {
            const baseFunc& derivedCast = derivedSub->cast<baseFunc>();
            std::string params = nul(derivedCast) ? "ctor" : _asStr(derivedCast.getParams());
            return posError(errCode::OBJ_WRONG_ARGS, me, i.name.c_str(),
                me.getArgs().asStr().c_str(), params.c_str());
        }

        a.setMe(nulOf<node>());
    }

    void me::onTraverse(runExpr& me, node& subject) {
        str ased = me.getMe() THEN(getEval()) orRet;

        getExpr& cast = subject.cast<getExpr>();
        if(!nul(cast)) cast.setMe(*ased);
    }

    std::string me::_asStr(const params& ps) {
        std::string ret;
        nbool first = true;
        for(const param& p: ps) {
            ret += (first ? "" : ",") + p.getOrigin().getType().getName();
            first = false;
        }

        return ret;
    }

    nbool me::onVisit(const visitInfo& i, ctor& me) {
        _GUARD("onVisit()");

        NM_WHENNUL(me.getRet()).thenErr(CTOR_NOT_IN_DEF_OBJ, me), true;

        frame& fr = thread::get()._getNowFrame();
        str prev = fr.getMe();
        fr.setMe(me.getRet()); // don't use 'cast<baseObj>'. it lets mockNode call 'cast' to its
                               // original instance.

        nbool ret = super::onVisit(i, me);
        fr.setMe(*prev);
        return ret;
    }

    void me::onLeave(const visitInfo& i, ctor& me) {
        _GUARD("onLeave()");

        _STEP("no error allowed during running ctor");
        const node& eval = *me.getBlock().getEval();
        NM_WHENNUL(eval).thenErr(EXPR_EVAL_NUL, me);
        NM_WHEN(eval.isSub<baseErr>()).thenErr(RET_ERR_ON_CTOR, me);

        me.outFrame(scope());
    }

    nbool me::onVisit(const visitInfo& i, func& me) {
        _GUARD("onVisit()");

        onLeave(i, (func::super&) me);

        obj& meObj =
            thread::get()
                ._getNowFrame()
                .getMe()
                .cast<obj>(); // TODO: same to 'thread::get().getNowFrame().getMe().cast<obj>();'
        NM_WHENNUL(meObj).thenErr(FUNC_REDIRECTED_OBJ, me), true;

        _STEP("check func duplication");
        const nbicontainer& top = meObj.getShares().getContainer();
        ncnt len = me.getParams().len();
        const node& errFound = top.get([&](const std::string& key, const node& val) {
            if(key != i.name) return false;
            if(&val == &me) return false;

            // this has same name on shares, but it's not func! it's not valid.
            // this could be an origin obj.
            const baseFunc& cast = val.cast<baseFunc>() orRet true;
            const params& castPs = cast.getParams();
            if(castPs.len() != len) return false;

            for(int n = 0; n < castPs.len(); n++) {
                str lhs = castPs[n].getOrigin().getEval();
                str rhs = me.getParams()[n].getOrigin().getEval();
                if(lhs->getType() != rhs->getType()) return false;
            }

            return true;
        });
        if(!nul(errFound)) posError(errCode::ALREADY_DEFINED_FUNC, me, i.name.c_str());

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
        str retType = me.getRet() orRet posError(errCode::NO_RET_TYPE, me), true;
        NM_WHEN(!retType->isSub(ttype<node>::get())).thenErr(WRONG_RET_TYPE, me, retType), true;

        blockExpr& blk = (blockExpr&) me.getBlock();
        if(nul(blk) || blk.getStmts().len() <= 0) {
            if(i.name == starter::MAIN) posError(errCode::MAIN_SHOULD_HAVE_STMTS, blk);
            /*TODO: uncomment after implement abstract:
            if(!func.isAbstract() && !retType->isSub<nVoid>())
                error(...)
            */
            return true;
        }

        _STEP("'break' or 'next' can't be used to last stmt");
        const node& lastStmt = *blk.getStmts().last();
        NM_WHEN(lastStmt.isSub<retStateExpr>() && !lastStmt.isSub<retExpr>())
            .thenErr(FUNC_SHOULD_RETURN_SOMETHING, lastStmt),
            true;

        _STEP("func[%s]: %s iterateBlock[%d]", i, me, me._blk->subs().len());

        // sequence of adding frame matters:
        //  object scope was added at 'onVisit(visitInfo, baseObj&)
        //  parameters of func is second:
        scope* s = new scope();
        for(const auto& p: me.getParams()) {
            if(p.getOrigin().isSub<nVoid>()) {
                posError(errCode::PARAM_NOT_VOID, me, p.getName().c_str());
                continue;
            }
            str eval = p THEN(getOrigin()) THEN(getEval()) orContinue;
            s->add(p.getName(), *new mockNode(*eval));
        }

        //  function's subs are third:
        me.inFrame(*s);
        return true;
    }

    nbool me::onVisit(const visitInfo& i, closure& me) {
        _GUARD("onVisit()");

        _STEP("is me captured?");
        NM_WHENNUL(me.getOrigin()).thenErr(NOT_CAPTURED, me), true;

        _STEP("does it have a func info?");
        NM_WHENNUL(me.getFunc()).thenErr(NOT_HAS_FUNC, me), true;
        return true;
    }

    void me::_prepare() {
        super::_prepare();
        _recentLoops.clear();
    }

    void me::_onEndWork() {
        for(baseObj* org: _orgs)
            org->setState(VERIFIED);

        super::_onEndWork();
    }

    void me::onLeave(const visitInfo& i, func& me) {
        _GUARD("onLeave()");

        _STEP("last stmt should match to ret type");
        const type& retType = me.getRet() THEN(getType()) orRet NM_E("func.getRet() is null");
        const node& lastStmt = *me.getBlock().getStmts().last();

        if(retType == ttype<nVoid>::get())
            return NM_I("func: implicit return won't verify NM_WHEN retType is void."), void();

        str eval = me.getBlock().getEval();
        NM_WHEN(!eval).thenErr(NO_RET_TYPE, lastStmt);

        const ntype& lastType = eval->getType(); // to get type of expr, always uses evalType.
        NM_WHENNUL(lastType).thenErr(NO_RET_TYPE, lastStmt);

        _STEP("last stmt[%s] should matches to return type[%s]", eval, retType);
        if(eval->isSub<retStateExpr>())
            // @see retExpr::getEval() for more info.
            return NM_I("func: skip verification NM_WHEN lastStmt is retStateExpr."), void();
        NM_WHEN(!lastType.isSub<baseErr>() && !lastType.isImpli(retType))
            .thenErr(RET_TYPE_NOT_MATCH, nul(lastStmt) ? me : lastStmt, lastType, retType);

        me.outFrame(scope());
    }

    nbool me::onVisit(const visitInfo& i, baseObj& me) {
        _GUARD("onVisit()");

        me.inFrame();
        frame& fr = thread::get()._getNowFrame();
        if(!me.isComplete()) {
            fr.subs().del("me");
            fr.subs().add("me", new mockNode(me));
        }

        _STEP("%s push me[%s] len=%d", fr.getMe(), (void*) &fr.getMe(), me.subs().len());

        _STEP("iterate all subs and checks void type variable");
        for(const node& elem: me.subs())
            if(elem.isSub<nVoid>()) posError(errCode::VOID_CANT_DEFINED, elem);

        _STEP("did user set the name of this object like 'const'?");
        NM_WHEN(util::checkTypeAttr(i.name) == ATTR_CONST && i.name.length() > 1)
            .thenErr(ORIGIN_OBJ_CANT_BE_CONST, me),
            true;

        _STEP("if obj is complete, does it have ctor without params?");
        if(me.isComplete()) {
            NM_WHENNUL(me.sub(baseObj::CTOR_NAME, args{})).thenErr(COMPLETE_OBJ_BUT_NO_CTOR, me),
                true;
        } else {
            _STEP("if me's origin is obj & incomplete, it shouldn't have any callComplete");
            const obj& org = me.getOrigin().cast<obj>();
            NM_WHEN(!nul(org) && !nul(org.getCallComplete()))
                .thenErr(CANT_CALL_COMPLETE_FOR_INCOMPLETE, me),
                true;
        }

        onLeave(i, (baseObj::super&) me);
        return true;
    }

    void me::onLeave(const visitInfo& i, baseObj& me) {
        _GUARD("onLeave()");
        me.outFrame();
        _orgs.push_back(&me);
    }

    nbool me::onVisit(const visitInfo& i, genericOrigin& me) {
        _GUARD("onVisit()");

        _STEP("cache check");
        for(const auto& e: me._cache)
            if(nul(e.second)) posError(errCode::MAKE_GENERIC_FAIL, me, e.first.c_str());

        _STEP("did user set the name of this object like 'const'?");
        NM_WHEN(util::checkTypeAttr(i.name) == ATTR_CONST && i.name.length() > 1)
            .thenErr(ORIGIN_OBJ_CANT_BE_CONST, me),
            true;

        _STEP("if obj is complete, does it have ctor without params?");
        if(me.isComplete())
            NM_WHENNUL(me.sub(baseObj::CTOR_NAME, args{})).thenErr(COMPLETE_OBJ_BUT_NO_CTOR, me),
                true;

        return true;
    }

    void me::onLeave(const visitInfo& i, genericOrigin& me) {
        _GUARD("onLeave()");

        _orgs.push_back(&me);
        // DON'T CALL 'super::onLeave()':
        //  if I don't have this func, getGenericExpr::super (=baseObj)'s one will be called.
        //  and me pointer will be erased too inside the func.
    }

    nbool me::onVisit(const visitInfo& i, forExpr& me) {
        _GUARD("onVisit()");
        _recentLoops.push_back(&me);

        str container = me._container;
        str conAsed = container->getEval();
        NM_WHEN(!conAsed).thenErr(CONTAINER_IS_NUL, me), true;
        str elemType = conAsed->run("getElemType");
        NM_WHEN(!elemType).thenErr(ELEM_TYPE_IS_NUL, me), true;

        const std::string& name = me.getLocalName();
        _STEP("define iterator '%s %s'", elemType, name);

        scope* s = new scope();
        s->add(name, *((node*) elemType->clone()));

        thread::get()._getNowFrame().add(*s);
        return true;
    }

    void me::onLeave(const visitInfo& i, forExpr& me) {
        _GUARD("onLeave()");

        thread::get()._getNowFrame().del(); // for a scope containing iterator.
        _onLeave(i, me);
    }

    nbool me::onVisit(const visitInfo& i, whileExpr& me) {
        _GUARD("onVisit()");
        _recentLoops.push_back(&me);
        return true;
    }

    void me::onLeave(const visitInfo& i, breakExpr& me) {
        _GUARD("onVisit()");

        _STEP("declared outside of loop?");
        NM_WHEN(_recentLoops.size() <= 0).thenErr(BREAK_OUTSIDE_OF_LOOP, me);
    }

    void me::onLeave(const visitInfo& i, nextExpr& me) {
        _GUARD("onVisit()");

        _STEP("declared outside of loop?");
        NM_WHEN(_recentLoops.size() <= 0).thenErr(NEXT_OUTSIDE_OF_LOOP, me);
    }

    nbool me::onVisit(const visitInfo& i, ifExpr& me) {
        _GUARD("onVisit()");
        return true;
    }

    void me::onLeave(const visitInfo& i, ifExpr& me) { _GUARD("onLeave()"); }
} // namespace nm
