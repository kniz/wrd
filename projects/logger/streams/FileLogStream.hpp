#pragma once

#include "Stream.hpp"

namespace wrd
{
    class FileLogStream : public Stream {
    public:
        FileLogStream();
        FileLogStream(const std::string& new_path);

    public:
        virtual const char* getName() const;
        virtual wbool initialize();
        wbool setPath(const std::string& new_path);
        const std::string& getPath() const;
        virtual wbool dump(const char* message);
        virtual wbool release();

    private:
        AsciiStream _file;
    };
}
