#pragma once

#include "errCode.hpp"
#include "../ast/point.hpp"
#include "../common/wrdMetaExtension.hpp"
#include "../type/wtype.hpp"
#include <unordered_map>

namespace wrd {

    typedef std::unordered_map<widx, std::string> msgMap;

    struct _wout err : public instance {
        WRD(ME(err, instance),
            TYPE(wtype),
            INIT_META(me))

    public:
        enum type {
            ERR = 0,
            WARN,
            INFO,
        };

    public:
        err(err::type t, wint newCode, ...);
        err(err::type t, int newCode, va_list args);

    public:
        virtual void log() const;
        void dbgLog() const;

        static const std::string& getErrMsg(errCode code);
        static const std::string& getErrName(errCode code);
        static err* newErr(int code, ...);
        static err* newErr(const area& src, int code, ...);
        static err* newWarn(int code, ...);
        static err* newWarn(const area& src, int code, ...);
        static err* newInfo(int code, ...);
        static err* newInfo(const area& src, int code, ...);

    public:
        err::type fType;
        errCode code;
        std::string msg;
        static constexpr wint BASE_TEST_CODE = 99999990; // not to be duplicated.

    private:
        std::string _format(const std::string& fmt, va_list args);
    };

    struct _wout dummyErr : public err {
        WRD(ME(dummyErr, err),
            TYPE(wtype),
            INIT_META(me))

    public:
        dummyErr();

    public:
        void log() const override;
    };

    struct _wout srcErr : public err {
        WRD(ME(srcErr, err),
            TYPE(wtype),
            INIT_META(me))

    public:
        template <typename... Args>
        srcErr(err::type t, const area& src, int code, Args... args): super(t, code, args...), srcArea(src) {}

        area srcArea;

    public:
        void log() const override;
    };
}
