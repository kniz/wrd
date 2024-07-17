#pragma once

#include "tcppBridgeFunc.hpp"
#include "tcppBridge.hpp"

namespace nm {

#define TEMPL template <typename Ret, typename T, nbool isBaseObj, template <typename, nbool> class Marshaling, typename... Args>
#define ME tcppBridgeFunc<Ret, T, isBaseObj, Marshaling, Args...>

    TEMPL
    template <size_t... index>
    str ME::_marshal(args& a, std::index_sequence<index...> s) {
        auto* me = (tcppBridge<T>*) &a.getMe();
        if(nul(me)) return NM_E("object from frame does not exists."), str();
        if(me->template isSub<mockNode>()) {
            mockNode& mock = me->template cast<mockNode>();
            me = (tcppBridge<T>*) &mock.getTarget();
        }
        if(nul(me->_real)) return NM_E("this object doesn't have _real."), str();

        return Marshaling<Ret, tifSub<Ret, node>::is>::toMgd((me->_real->*(this->_fptr)) // funcptr
                (Marshaling<Args, tifSub<Args, node>::is>::toNative(a[index])...)); // and args.
    }

#undef ME
#undef TEMPL
#define TEMPL template <typename T, template <typename, nbool> class Marshaling, typename... Args>
#define ME tcppBridgeFunc<void, T, false, Marshaling, Args...>

    TEMPL
    template <size_t... index>
    str ME::_marshal(args& a, std::index_sequence<index...>) {
        auto* me = (tcppBridge<T>*) &a.getMe();
        if(nul(me)) return NM_E("object from frame does not exists."), str();
        if(me->template isSub<mockNode>()) {
            mockNode& mock = me->template cast<mockNode>();
            me = (tcppBridge<T>*) &mock.getTarget();
        }
        if(nul(me->_real)) return NM_E("this object doesn't have _real."), str();

        (me->_real->*(this->_fptr))(Marshaling<Args, tifSub<Args, node>::is>::toNative(a[index])...);
        return Marshaling<void, tifSub<void, node>::is>::toMgd();
    }

#undef TEMPL
#undef ME
}
