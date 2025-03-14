#include "baseFunc.hpp"

#include "../builtin/primitive/nByte.hpp"
#include "../builtin/primitive/nInt.hpp"
#include "../frame/frame.hpp"
#include "../frame/frameInteract.hpp"
#include "../worker/visitor/visitor.hpp"
#include "../type/as/ases.hpp"
#include "params.hpp"
#include "closure.hpp"

namespace nm {

    NM(DEF_ME(baseFunc), DEF_VISIT())

    me::baseFunc(const modifier& mod): _mod(mod) {}

    scope& me::subs() {
        static dumScope inner;
        return inner;
    }

    priorType me::prioritize(const args& a) const {
        NM_DI("%s.prioritize(%s)", *this, nul(a) ? "()" : a.asStr());
        const params& ps = getParams();
        if(a.len() != ps.len()) return NO_MATCH;

        int n = 0;
        priorType max = EXACT_MATCH; // begining from lv0.
        for(const auto& e: a) {
            str t = e.getEval() orRet NM_W("t == null"), NO_MATCH;
            str p = ps[n++].getOrigin().as<node>();
            if(!t->isComplete()) return NO_MATCH;

            // overloading priority algorithm:
            //  each subs can be categorized into 3 level of priority.
            //  the priority against func call will be computed maximum priority value between the
            //  argument and its paramter. the lower value of 'max', the more check needed.
            priorType newP = _prioritize(*p, *t);
            max = newP > max ? newP : max;
            if(max == NO_MATCH) return NO_MATCH;
        }

        return max;
    }

    priorType me::_prioritize(const node& param, const node& arg) const {
        if(nul(param) || nul(arg)) return NO_MATCH;
        if(arg.getType() == param.getType()) return EXACT_MATCH;
        if(_isNatureNumber(param) && _isNatureNumber(arg)) return NUMERIC_MATCH;
        if(arg.isImpli(param)) return IMPLICIT_MATCH;

        return NO_MATCH;
    }

    void me::_setSrc(const src& newSrc) { _src.bind(newSrc); }

    nbool me::isFuncButNotClosure(const node& n) {
        if(nul(n)) return false;
        return n.isSub<baseFunc>() && !n.isSub<closure>();
    }

    ntype& me::_getType() { return (ntype&) getType(); }

    params& me::getParams() { return ((ntype&) getType()).getParams(); }

    const node& me::getRet() const { return getType().getRet(); }

    const src& me::getSrc() const {
        if(!_src) return super::getSrc();
        return *_src;
    }

    const modifier& me::getModifier() const {
        if(_mod) return *_mod;
        return super::getModifier();
    }

    nbool me::_isNatureNumber(const node& it) const {
        return it.isSub<nInt>() || it.isSub<nByte>();
    }
} // namespace nm
