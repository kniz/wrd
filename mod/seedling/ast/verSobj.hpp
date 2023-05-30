#pragma once

#include "termSobj.hpp"

namespace namu {

    class _nout verSobj : public termSobj {
        NAMU_DECL_ME(verSobj, termSobj)
        NAMU_INIT_META(verSobj)

    public:
        verSobj(int major, int minor, int fix);
        explicit verSobj(const std::string& verStr);
        explicit verSobj(const nchar* verStr);

    public:
        nbool operator>(const me& rhs) const;
        nbool operator<(const me& rhs) const;
        nbool operator==(const me& rhs) const;
        nbool operator!=(const me& rhs) const;
        nbool operator<=(const me& rhs) const;
        nbool operator>=(const me& rhs) const;

    public:
        nint asMajor() const;
        nint asMinor() const;
        nint asFix() const;
        const type& getType() const override;

    private:
        static nint _isFromBigger(nint from, nint to);
        void _parseVerStr(const std::string& verStr);

    private:
        nint _maj;
        nint _min;
        nint _fix;
        static constexpr nint VER_LEN = 3;
        static constexpr const nchar* DELIMITER = ".";
    };
}

