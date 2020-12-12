#pragma once

#include "stream/Stream.hpp"

namespace wrd { namespace clog {

    class Logger : public Stream {
        WRD_DECL_THIS(Logger, Stream)

    public:
        virtual const wchar* getName() const;
        const Stream& operator[](widx n) const;
        Stream& operator[](widx n);
        const Stream& operator[](const wchar* message) const;
        Stream& operator[](const wchar* message);

        Stream& getStream(widx n);
        const Stream& getStream(widx n) const;
        const Stream& getStream(const wchar* c_message) const;
        Stream& getStream(const wchar* message);
        wcnt getStreamCount() const;
        virtual wbool dump(const wchar* message);
        wbool dumpFormat(const wchar* fmt, ...);
        wbool pushStream(Stream* new_stream);
        virtual wbool isInit() const;
        static Logger& getInstance();

    private:
        Logger();
        Logger(const Logger& rhs);
        virtual wbool init();
        virtual wbool release();

    private:
        std::vector<Stream*> _streams;
    };
} }
