#pragma once

#include "tstr.hpp"

namespace nm {
    template <typename T> static T* __proceed__(const tweak<T>&& rhs) {
        static tweak<T> a;
        a = rhs;
        return &a.get();
    }

    template <typename T> struct nulr<tstr<T>> {
        static tstr<T> get() { return tstr<T>(); }

        static nbool isNul(const tstr<T>& it) { return nul(&it) || !it.isBind(); }
    };

    template <typename T> struct nulr<tstr<T>&> {
        static tstr<T>& get() {
            tstr<T>* ret = nullptr;
            return *ret;
        }

        static nbool isNul(const tstr<T>& it) { return nul(&it) || !it.isBind(); }
    };

    template <typename T> struct __unwrap_binder__<tstr<T>> {
        static T& to(tstr<T> it) { return *it; }
    };

    template <typename T> struct __unwrap_binder__<tstr<T>&> {
        static T& to(tstr<T>& it) { return *it; }
    };

    template <typename T> struct __unwrap_binder__<tstr<T>*> {
        static T& to(tstr<T>* it) { return it->get(); }
    };

    template <typename T> struct __unwrap_binder__<tweak<T>> {
        static T& to(tstr<T> it) { return *it; }
    };

    template <typename T> struct __unwrap_binder__<tweak<T>&> {
        static T& to(tstr<T>& it) { return *it; }
    };

    template <typename T> struct __unwrap_binder__<tweak<T>*> {
        static T& to(tstr<T>* it) { return it->get(); }
    };

    template <typename T> static T* __proceed__(tweak<T>& rhs) { return &rhs.get(); }

    template <typename T> static T* __proceed__(tweak<T>* rhs) { return &rhs->get(); }

    template <typename T> static T* __proceed__(const tweak<T>& rhs) { return &rhs.get(); }

    template <typename T> static T* __proceed__(const tweak<T>* rhs) { return &rhs->get(); }

    template <typename T> static T* __proceed__(const tstr<T>&& rhs) {
        static tstr<T> a;
        a = rhs;
        return &a.get();
    }

    template <typename T> static T* __proceed__(tstr<T>& rhs) { return &rhs.get(); }

    template <typename T> static T* __proceed__(tstr<T>* rhs) { return &rhs->get(); }

    template <typename T> static T* __proceed__(const tstr<T>& rhs) { return &rhs.get(); }

    template <typename T> static T* __proceed__(const tstr<T>* rhs) { return &rhs->get(); }

    template <typename T, typename TACTIC> static nbool __isNul__(const tstr<T, TACTIC>&& rhs) {
        return !&rhs || !rhs;
    }

    template <typename T, typename TACTIC> static nbool __isNul__(const tstr<T, TACTIC>& rhs) {
        return !&rhs || !rhs;
    }

    template <typename T, typename TACTIC> static nbool __isNul__(const tstr<T, TACTIC>* rhs) {
        return !rhs || !*rhs;
    }

    template <typename T, typename TACTIC> static nbool __isNul__(const tweak<T, TACTIC>&& rhs) {
        return !&rhs || !rhs;
    }

    template <typename T, typename TACTIC> static nbool __isNul__(const tweak<T, TACTIC>& rhs) {
        return !&rhs || !rhs;
    }

    template <typename T, typename TACTIC> static nbool __isNul__(const tweak<T, TACTIC>* rhs) {
        return !rhs || !*rhs;
    }

    template <typename T> struct __to_ref__<tstr<T>> {
        static T& to(tstr<T> it) { return *it; }
    };

    template <typename T> struct __to_ref__<tstr<T>&> {
        static T& to(tstr<T>& it) { return *it; }
    };

    template <typename T> struct __to_ref__<tstr<T>*> {
        static T& to(tstr<T>* it) { return it->get(); }
    };

    template <typename T> struct __to_ref__<const tstr<T>&> {
        static const T& to(const tstr<T>& it) { return *it; }
    };

    template <typename T> struct __to_ref__<const tstr<T>*> {
        static const T& to(const tstr<T>* it) { return it->get(); }
    };

    template <typename T> struct __to_ref__<tstr<T>&&> {
        static T& to(tstr<T>&& it) { return *it; }
    };
} // namespace nm
