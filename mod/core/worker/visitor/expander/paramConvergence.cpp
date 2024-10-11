#include "paramConvergence.hpp"
#include "../../../ast/param.hpp"
#include "../../../ast/func.hpp"
#include "../../../frame/thread.hpp"

namespace nm {
    NM(DEF_ME(paramConvergence))

    me::paramConvergence(baseFunc& f, param& p, const node& org): _f(&f), _p(&p), _org(&org) {}

    nbool me::converge() const {
        if(!_org || !_p || !_f) return NM_E("some paramConvergence info is null"), false;

        str eval = _org->getEval() orRet false;
        const node& owner = _f->getOrigin() orRet false;

        const frame& fr = thread::get().getNowFrame();
        if(&fr.getOwner(*eval) != &owner) return false;

        _p->setOrigin(*eval);
        return true;
    }
}
