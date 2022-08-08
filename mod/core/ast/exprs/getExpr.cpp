#include "getExpr.hpp"
#include "../../loader/interpreter/tverification.hpp"
#include "../../loader/interpreter/verification.inl"
#include "../../loader/interpreter/verifier.hpp"
#include "../../frame/thread.hpp"
#include "../node.inl"

namespace namu {

    NAMU_DEF_ME(getExpr)

    const node& me::getFrom() const {
        if(!_from)
            return thread::get().getNowFrame();
        return *_from;
    }

    const node& me::getEval() const {
        const node& got = _get();
        if(nul(got))
            return nulOf<node>();
        return got.getEval();
    }

    str me::run(const args& a) {
        // believe that this expression was checked to be valid.
        return str(_get());
    }

    const std::string& me::getSubName() const { return _name; }

    /// @return nullable
    const args& me::getSubArgs() const { return *_args; }

    node& me::_get() const {
        str evalMe = getFrom().isSub<expr>() ? getFrom().as<node>() : getFrom();
        if(!evalMe) return NAMU_E("from == null"), nulOf<node>();
        NAMU_DI("_name=%s", _name.c_str());
        if(!_args) return evalMe->sub(_name);

        return evalMe->sub(_name, *_args);
    }



    NAMU_VERIFY(getExpr, isRunnable, {
        // TODO: I have to check that the evalType has what matched to given _params.
        // Until then, I rather use as() func and it makes slow emmersively.
        if(nul(it.getEval())) return _srcErr(errCode::EVAL_NULL_TYPE);
        const node& got = it._get();
        if(nul(got)) {
            const node& from = it.getFrom();
            return _srcErr(errCode::CANT_ACCESS, from.getType().getName().c_str(), it._name.c_str());

        NAMU_DI("verify: getExpr: isRunnable: got=%s, it=%s", got.getType().getName().c_str(),
                it.getType().getName().c_str());
        }
    })

    NAMU_VERIFY({
        NAMU_DI("verify: getExpr: visit 'from' subnodes");

        verify((node&) it.getFrom());
    })
}
