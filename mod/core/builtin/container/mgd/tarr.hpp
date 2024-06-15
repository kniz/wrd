#pragma once

#include "arr.hpp"

namespace namu {

    template <typename T, typename defaultElemType = T>
    class tarr : public arr {
        NAMU(ME(tarr, arr),
             INIT_META(tarr),
             CLONE(tarr))

    public:
        tarr(): super(*new defaultElemType()) {}
        tarr(const node& elemType): super(elemType) {}
    };
}
