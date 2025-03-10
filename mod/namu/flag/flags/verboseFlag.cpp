#include "verboseFlag.hpp"

#include "../../cli.hpp"

namespace nm {

    NM_DEF_ME(verboseFlag)

    const nchar* me::getName() const { return "-v"; }

    const nchar* me::getDescription() const {
        return R"DESC(
    prints all logs in verbose level of the interpreter.
    these are mostly about parser and verifier logs. and it includes all of process
    to tokenizing, parsing, verification.
    this info will be useful if you know how src to be interpreted.)DESC";
    }

    const strings& me::_getRegExpr() const {
        static strings inner{"^\\-v$"};
        return inner;
    }

    nbool me::_onTake(const flagArgs& tray, cli& c, interpreter& ip, starter& s) const {
        nint flag = interpreter::DUMP_ON_EX | interpreter::GUARD | interpreter::INTERNAL |
            interpreter::LOG_ON_END | interpreter::LOG_GRAPH_ON_EX;
        ip.setFlag(flag);
        s.setFlag(flag);
        return true;
    }
}
