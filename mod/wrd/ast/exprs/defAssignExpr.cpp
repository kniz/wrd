#include "defAssignExpr.hpp"
#include "../../frame/thread.hpp"
#include "../../loader/interpreter/tverification.hpp"
#include "../../loader/interpreter/verification.inl"
#include "../../loader/interpreter/verifier.hpp"

namespace wrd {
    WRD_DEF_ME(defAssignExpr)

    str me::run(const ucontainable& args) {
        str new1 = _rhs->run();
        if(!new1)
            return WRD_E("new1 is null"), str();

        node& to = getTo();
        if(nul(to))
            thread::get()._getNowFrame().pushLocal(_name, *new1);
        else
            to.run()->subs().add(_name, *new1);

        return new1;
    }

    node& me::getTo() { return *_to; }



    WRD_VERIFY({
        WRD_DI("verify: defAssignExpr: duplication of variable.");

        const scopes& top = thread::get().getNowFrame().getTop();
        if(nul(top)) return;
        const std::string name = it.getSubName();
        if(top.getContainer().has(name))
            return _srcErr(errCode::ALREADY_DEFINED_VAR, name.c_str(), it.getEval().getType().getName()
                    .c_str());
    })

    WRD_VERIFY(defAssignExpr, isDefinable, {
        WRD_DI("verify: defAssignExpr: is definable?");

        const node& rhs = it.getRight();
        if(nul(rhs))
            return _srcErr(errCode::CANT_DEF_VAR, it.getSubName().c_str(), "null");

        WRD_DI("verify: defAssignExpr: %s has defined.",
                nul(rhs) ? "name" : rhs.getType().getName().c_str());

        node& to = it.getTo();
        str new1 = rhs.as<node>();
        if(nul(to))
            thread::get()._getNowFrame().pushLocal(it.getSubName(), *new1);
        else
            to.run()->subs().add(it.getSubName(), *new1);
    })

    WRD_VERIFY({
        WRD_DI("verify: defAssignExpr: visitSubNodes");

        verify(it.getRight());
    })
}
