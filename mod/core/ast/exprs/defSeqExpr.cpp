#include "defSeqExpr.hpp"
#include "../../visitor/visitor.hpp"
#include "../../builtin/container/mgd/seq.hpp"

namespace namu {

    NAMU(DEF_ME(defSeqExpr), DEF_VISIT())

    me::defSeqExpr(const node& start, const node& end): _start(start), _end(end) {}

    str me::run(const args& a) {
        if(!_start || !_end)
            return NAMU_E("_start or _end is null"), str();

        str start(_start->as<node>());
        str end(_end->as<node>());
        if(!start || !end)
            return NAMU_E("ased start or end is null"), str();

        return str(new seq(start->cast<nint>(), end->cast<nint>()));
    }

    str me::getEval() const {
        static str inner(new seq(1, 10));
        return inner;
    }

    const node& me::getStart() const {
        return *_start;
    }

    const node& me::getEnd() const {
        return *_end;
    }
}
