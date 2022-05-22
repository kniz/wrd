#pragma once

#include "../tbicontainable.hpp"

namespace wrd {

    template <typename K, typename V>
    class tnbicontainer : public instance, public tbicontainable<K, V>, public clonable {
        WRD(ADT(tnbicontainer, instance))
    };

    typedef tnbicontainer<std::string, node> nbicontainer;
}
