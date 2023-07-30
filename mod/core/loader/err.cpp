#include "../ast/node.hpp"
#include "err.hpp"

namespace namu {

    NAMU_DEF_ME(err)

    namespace {
        constexpr nint MAX_BUF = 512;
    }

    const std::string& err::getErrMsg(errCode code) {
        static std::string msg[] = {
            "start",
#define PAIRX PAIRX_RIGHT
#include "errCode.inl"
#undef PAIRX
            "end",
        };

        if(code < ERR_CODE_START) code = ERR_CODE_START;
        if(code > ERR_CODE_END) code = ERR_CODE_END;

        return msg[code];
    }

    const std::string& err::getErrName(errCode code) {
        static std::string msg[] = {
            "ERR_CODE_START",
#define PAIRX(A, B) #A,
#include "errCode.inl"
#undef PAIRX
            "ERR_CODE_END",
        };

        if(code < ERR_CODE_START) code = ERR_CODE_START;
        if(code > ERR_CODE_END) code = ERR_CODE_END;

        return msg[code];
    }

    std::string me::_format(const std::string& fmt, va_list args) {
        nchar buf[MAX_BUF] = {0, };
        vsnprintf(buf, MAX_BUF, fmt.c_str(), args);

        return buf;
    }

#define _EXPAND_VA(EXPR) \
        va_list args; \
        va_start(args, code); \
        err* ret = EXPR; \
        va_end(args); \
        return ret

    err* me::newErr(int code, ...) {
        _EXPAND_VA(new err(err::ERR, code, args));
    }
    err* me::newErr(const point& pos, int code, ...) {
        _EXPAND_VA(new err(err::ERR, pos, code, args));
    }
    err* me::newWarn(int code, ...) {
        _EXPAND_VA(new err(err::WARN, code, args));
    }
    err* me::newWarn(const point& pos, int code, ...) {
        _EXPAND_VA(new err(err::WARN, pos, code, args));
    }
    err* me::newInfo(int code, ...) {
        _EXPAND_VA(new err(err::INFO, code, args));
    }
    err* me::newInfo(const point& pos, int code, ...) {
        _EXPAND_VA(new err(err::INFO, pos, code, args));
    }

#undef _EXPAND_VA

    me::err(err::type t, nint newCode): super(), fType(t), code((errCode) newCode) {}

    me::err(err::type t, nint newCode, va_list args): super(), fType(t), code((errCode) newCode) {
        msg = _format(getErrMsg(code), args);
    }

    me::err(err::type t, const point& ps, nint newCode, va_list args)
        : super(), fType(t), code((errCode) newCode), pos(ps) {
        msg = _format(getErrMsg(code), args);
    }

    nbool me::operator==(const me& rhs) const {
        return fType == rhs.fType && code == rhs.code && code == rhs.code;
    }
    nbool me::operator!=(const me& rhs) const {
        return !operator==(rhs);
    }

    void me::log() const {
        auto& log = logger::get();
        switch(fType) {
            case ERR:
                std::cout << platformAPI::getConsoleFore(platformAPI::LIGHTRED);
                if(pos.isOrigin())
                    log.dumpFormat("err%d(%s)", code, getErrName(code).c_str());
                else
                    log.dumpFormat("err%d(%s) %d:%d", code, getErrName(code).c_str(), pos.row, pos.col);
                std::cout << platformAPI::getConsoleFore(platformAPI::LIGHTGRAY);
                log.dumpFormat(": %s\n", msg.c_str());
                break;

            case WARN:
                std::cout << platformAPI::getConsoleFore(platformAPI::YELLOW);
                if(pos.isOrigin())
                    log.dumpFormat("warn%d(%s)", code, getErrName(code).c_str());
                else
                    log.dumpFormat("warn%d(%s) %d:%d", code, getErrName(code).c_str(), pos.row, pos.col);
                std::cout << platformAPI::getConsoleFore(platformAPI::LIGHTGRAY);
                log.dumpFormat(": %s\n", msg.c_str());
                break;

            case INFO:
                std::cout << platformAPI::getConsoleFore(platformAPI::BLUE);
                if(pos.isOrigin())
                    log.dumpFormat("info%d(%s)", code, getErrName(code).c_str());
                else
                    log.dumpFormat("info%d(%s) %d:%d", code, getErrName(code).c_str(), pos.row, pos.col);
                std::cout << platformAPI::getConsoleFore(platformAPI::LIGHTGRAY);
                log.dumpFormat(": %s\n", msg.c_str());
                break;
        }
    }

    void me::dbgLog() const {
        if(buildFeature::config::isDbg())
            log();
    }

    dummyErr::dummyErr(): super(err::ERR, 0) {}

    void dummyErr::log() const {}
}
