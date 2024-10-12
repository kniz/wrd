#include "paramConvergence.hpp"
#include "../../../ast/param.hpp"
#include "../../../ast/func.hpp"
#include "../../../frame/thread.hpp"

namespace nm {
    NM(DEF_ME(paramConvergence))

    me::paramConvergence(baseObj& o, baseFunc& f, param& p, const node& org): super(o, f), _p(&p), _org(&org) {}

    nbool me::_onConverge(baseFunc& f) const {
        str eval = _org->getEval() orRet false;
        const node& owner = f.getOrigin() orRet false;

        const frame& fr = thread::get().getNowFrame();
        if(&fr.getOwner(*eval) != &owner) return false;

        _p->setOrigin(*eval);
        return true;
    }
}
