#include "mgdFunc.hpp"
#include "../obj.hpp"
#include "../../builtin/container/native/tnchain.inl"
#include "../../frame/thread.hpp"
#include "../../loader/interpreter/tverification.hpp"
#include "../params.hpp"

namespace wrd {

    WRD_DEF_ME(mgdFunc)

    me::mgdFunc(const params& ps, const wtype& retType):
            super(), _params(ps), _retType(&retType), _blk(new blockExpr()) {}
    me::mgdFunc(const params& ps, const wtype& retType, const blockExpr& newBlock):
            super(), _params(ps), _retType(&retType), _blk(newBlock) {}

    blockExpr& me::getBlock() { return *_blk; }
    const blockExpr& me::getBlock() const { return *_blk; }
    const wtype& me::getRetType() const { return *_retType; }
    nbicontainer& me::subs() { return _shares; }
    const params& me::getParams() const { return _params; }

    str me::run(const ucontainable& args) {
        if(!_inFrame(*_evalArgs(args)))
            return str();

        _blk->run();
        str ret = thread::get()._getNowFrame().popReturn();
        _outFrame();
        return ret;
    }

    scope* me::_evalArgs(const ucontainable& args) {
        const params& ps = getParams();
        if(args.len() != ps.len())
            return WRD_E("length of args(%d) and typs(%d) doesn't match.", args.len(), ps.len()), nullptr;

        scope* ret = new scope();
        int n = 0;
        for(const node& e: args) {
            const param& p = ps[n++];
            str evaluated = e.as(p.getOrigin());
            if(!evaluated) return nullptr;

            ret->add(p.getName(), *evaluated);
        }
        return ret;
    }

    wbool me::_inFrame(scope& s) {
        frame& fr = thread::get()._getNowFrame();
        if(nul(fr)) return WRD_E("fr == null"), false;
        if(nul(s)) return WRD_E("s == null"), false;

        WRD_DI("%s._onInFrame()", getType().getName().c_str());
        fr.pushLocal(subs());
        fr.setFunc(*this);
        return fr.pushLocal(s);
    }

    void me::_outFrame() {
        frame& fr = thread::get()._getNowFrame();
        WRD_DI("%s._onOutFrame()", getType().getName().c_str());
        fr.setFunc(nulOf<func>());
        fr.popLocal();
        fr.popLocal();
    }

    WRD_VERIFY({
        WRD_DI("verify: retType exists and stmts exist one at least");

        const wtype& retType = it.getRetType();
        if(nul(retType)) return _err(errCode::NO_RET_TYPE);
        if(!retType.isSub(ttype<node>::get()))
            return _err(errCode::WRONG_RET_TYPE, retType.getName().c_str());

        const blockExpr& blk = it.getBlock();
        if(nul(blk) || blk.getStmts().len() <= 0)
            return _err(errCode::NO_STMT_IN_FUNC);
    })

    namespace {
        void _prepareArgsAlongParam(const params& ps, scope& s) {
            for(const auto& p : ps)
                s.add(p.getName(), *p.getOrigin());
        }
    }

    // TODO: verify arguments

    WRD_VERIFY(mgdFunc, subNodes, {
        WRD_DI("verify: mgdFunc: %s iterateBlock[%d]", it.getType().getName().c_str(), it._blk->subs().len());
        scope* s = new scope();
        _prepareArgsAlongParam(it.getParams(), *s);

        it._inFrame(*s);

        verify(*it._blk);

        it._outFrame();
    })
}
