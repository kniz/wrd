#pragma once

#include "../builtin/container/native/tnarr.hpp"
#include "obj.hpp"

namespace namu {

    class args : public narr {
        NAMU(CLASS(args, narr))

    public:
        args() {}
        args(const baseObj& me): _me(me) {}
        args(const narr& rhs): super(rhs) {}
        args(const baseObj& me, const narr& rhs): super(rhs), _me(me) {}

    public:
        const me& setMe(const baseObj& me) const {
            _me.bind(me);
            return *this;
        }
        baseObj& getMe() const { return *_me; }

        std::string asStr() const {
            std::string ret;
            nbool first = true;
            for(const auto& e : *this) {
                ret += (first ? "" : ",") + e.getType().getName();
                first = false;
            }

            return ret;
        }

    private:
        mutable tstr<baseObj> _me;
    };
}
