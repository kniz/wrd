#include "defAssignExpr.hpp"

#include "../../worker/visitor/visitor.hpp"

namespace nm {
    NM(DEF_ME(defAssignExpr), DEF_VISIT())

    me::defAssignExpr(const std::string& name, const node& rhs): super(name, rhs) {}

    me::defAssignExpr(const std::string& name, const node& rhs, const node& to, const src& s,
        const modifier& mod):
        me(name, nulOf<node>(), rhs, to, s, mod) {}

    me::defAssignExpr(const std::string& name, const node& type, const node& rhs, const node& to,
        const src& s, const modifier& mod):
        super(name, rhs, to, s, mod), _type(type) {}

    tstr<baseObj> me::_onMakeNew() const { return getRight().as(*_type); }

    str me::getEval() const { return *_type; }
}
