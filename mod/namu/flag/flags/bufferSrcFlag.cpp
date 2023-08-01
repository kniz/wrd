#include "bufferSrcFlag.hpp"
#include "../../cli.hpp"
#include <iostream>

namespace namu {

    NAMU_DEF_ME(bufferSrcFlag)

    const nchar* me::getName() const {
        return "-s";
    }

    const nchar* me::getDescription() const {
        return R"DESC(
    programming script can passed in as a string with this flag.)DESC";
    }

    const nchar* me::_getRegExpr() const {
        return "^\\-s$";
    }

    ncnt me::_onContinuousArgCount() const {
        return 1;
    }

    nbool me::_onTake(const flagArgs& tray, cli& c, interpreter& ip) const {
        if(tray.size() < 2) return NAMU_E("invalid flagArgument size < 2"), false;

        std::vector<std::string> buf;
        buf.push_back(tray[1]);
        ip.setSrcSupply(*new bufferSrcSupply(buf));
        return true;
    }
}
