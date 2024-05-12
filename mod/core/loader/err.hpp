#pragma once

#include "errCode.hpp"
#include "../ast/point.hpp"
#include "../common/namuMetaExtension.hpp"
#include "../type/ntype.hpp"
#include "../type/dumpable.hpp"
#include "../builtin/container/native/tnbicontainer.hpp"
#include "../frame/callstack.hpp"
#include "../frame/frame.hpp"
#include <unordered_map>

namespace namu {

    typedef std::unordered_map<nidx, std::string> msgMap;

    class pos;
    class scopes;
    struct _nout err : public instance, public dumpable {
        NAMU(CLASS(err, instance))

    public:
        err(logLv::level t, nint newCode);
        err(logLv::level t, nint newCode, va_list args);
        err(logLv::level t, const point& ps, nint newCode, va_list args);
        err(const me& rhs);

    public:
        nbool operator==(const me& rhs) const;
        nbool operator!=(const me& rhs) const;

    public:
        virtual void log() const;
        void dbgLog() const;

        const callstack& getStack() const;
        void logStack() const;

        static const std::string& getErrMsg(errCode code);
        static const std::string& getErrName(errCode code);
        static err* newErr(int code, ...);
        static err* newErr(int code, va_list args);
        static err* newErr(const point& src, int code, ...);
        static err* newErr(const point& src, int code, va_list args);
        static err* newWarn(int code, ...);
        static err* newWarn(int code, va_list args);
        static err* newWarn(const point& src, int code, ...);
        static err* newWarn(const point& src, int code, va_list args);
        static err* newInfo(int code, ...);
        static err* newInfo(int code, va_list args);
        static err* newInfo(const point& src, int code, ...);
        static err* newInfo(const point& src, int code, va_list args);

        void dump() const override;

    private:
        std::string _format(const std::string& fmt, va_list args);
        void _logNativeStack() const;

    public:
        logLv::level fType;
        errCode code;
        std::string msg;
        point pos;
        callstack _stack;
        static constexpr nint BASE_TEST_CODE = 99999990; // not to be duplicated.
    };

    struct _nout dummyErr : public err {
        NAMU(CLASS(dummyErr, err))

    public:
        dummyErr();

    public:
        void log() const override;
    };
}
