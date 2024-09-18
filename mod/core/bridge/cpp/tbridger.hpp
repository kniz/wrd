#pragma once

#include "../../ast/obj.hpp"
#include "../../ast/tbaseObjOrigin.hpp"
#include "../../type/mgdType.hpp"
#include "marshaling/tgenericMarshaling.hpp"
#include "tbridgeClosure.hpp"
#include "tbridgeCtor.hpp"
#include "tbridgeFunc.hpp"

namespace nm {

    template <typename T, nbool isBaseObj = tifSub<typename tadaptiveSuper<T>::super, baseObj>::is>
    class tbridger {
        NM(ME(tbridger))
        template <typename Ret, typename T1, nbool, template <typename, nbool> class Marshaling,
            typename... Args>
        friend class tbridgeFunc;
        template <typename T1> friend class tbridge;

    private:
        tbridger() = default;

    public:
        static scope& subs() { return _get()._subs; }

        static me& extend(const scope& src) {
            subs().link(src);
            return _get();
        }

        static me& func(const std::string& name, const baseFunc& bridgeFunc) {
            _get().subs().add(name, bridgeFunc);
            return _get();
        }

        static me& func(const std::string& name, const baseFunc* bridgeFunc) {
            return func(name, *bridgeFunc);
        }

        template <typename... Args> static me& ctor() {
            return func(baseObj::CTOR_NAME, new tbridgeCtor<T, Args...>());
        }

        template <typename Ret, typename T1 = T, typename... Args>
        static me& closure(const std::string& name, std::function<Ret(T1&, Args...)> c) {
            return func(name, new tbridgeClosure<Ret, T1, tmarshaling, Args...>(c));
        }

        template <typename Ret, typename T1 = T>
        static me& closure(const std::string& name, std::function<Ret(T1&)> c) {
            return func(name, new tbridgeClosure<Ret, T1, tmarshaling>(c));
        }

        template <typename Ret, typename... Args>
        static me& func(const std::string& name, Ret (T::*fptr)(Args...)) {
            return funcNonConst(name, fptr);
        }

        template <typename Ret, typename... Args>
        static me& func(const std::string& name, Ret (T::*fptr)(Args...) const) {
            return funcConst(name, fptr);
        }

        template <typename Ret, typename... Args>
        static me& funcNonConst(const std::string& name, Ret (T::*fptr)(Args...)) {
            return func(name, new tbridgeFunc<Ret, T, isBaseObj, tmarshaling, Args...>(fptr));
        }

        template <typename Ret, typename... Args>
        static me& funcConst(const std::string& name, Ret (T::*fptr)(Args...) const) {
            return func(name,
                new tbridgeFunc<Ret, T, isBaseObj, tmarshaling, Args...>(
                    (Ret(T::*)(Args...)) fptr));
        }

        template <typename Ret, typename... Args>
        static me& genericFunc(const std::string& name, Ret (T::*fptr)(Args...)) {
            return func(name,
                new tbridgeFunc<Ret, T, isBaseObj, tgenericMarshaling, Args...>(fptr));
        }

        template <typename Ret, typename... Args>
        static me& genericFuncNonConst(const std::string& name, Ret (T::*fptr)(Args...)) {
            return func(name,
                new tbridgeFunc<Ret, T, isBaseObj, tgenericMarshaling, Args...>(
                    (Ret(T::*)(Args...)) fptr));
        }

        template <typename Ret, typename... Args>
        static me& genericFunc(const std::string& name, Ret (T::*fptr)(Args...) const) {
            return func(name,
                new tbridgeFunc<Ret, T, isBaseObj, tgenericMarshaling, Args...>(
                    (Ret(T::*)(Args...)) fptr));
        }

        template <typename Ret, typename... Args>
        static me& genericFuncConst(const std::string& name, Ret (T::*fptr)(Args...) const) {
            return func(name,
                new tbridgeFunc<Ret, T, isBaseObj, tgenericMarshaling, Args...>(
                    (Ret(T::*)(Args...)) fptr));
        }

        static tbridge<T>* make(T* real) {
            static tbaseObjOrigin<tbridge<T>> org(_get().subs());
            return new tbridge(real, org);
        }

    private:
        static me& _get() {
            static me _inner;
            return _inner;
        }

    private:
        scope _subs;
    };

    template <typename T> class tbridger<T, true> {
        NM(ME(tbridger))

    private:
        tbridger() = default;

    public:
        static scope& subs() { return _get()._subs; }

        static me& extend(const scope& src) {
            subs().link(src);
            return _get();
        }

        static me& func(const std::string& name, const baseFunc& bridgeFunc) {
            _get().subs().add(name, bridgeFunc);
            return _get();
        }

        static me& func(const std::string& name, const baseFunc* bridgeFunc) {
            return func(name, *bridgeFunc);
        }

        static me& ctor() { return func(baseObj::CTOR_NAME, new tbridgeCtor<T>()); }

        template <typename... Args> static me& ctor() {
            return func(baseObj::CTOR_NAME, new tbridgeCtor<T, Args...>());
        }

        /*template <typename T1, typename... Args> static me& ctorIndirect() {
            return func(baseObj::CTOR_NAME, new tbridgeCtor<T1, Args...>());
        }*/

        template <typename Ret, typename T1 = T, typename... Args>
        static me& closure(const std::string& name, std::function<Ret(T1&, Args...)> c) {
            return func(name, new tbridgeClosure<Ret, T1, tmarshaling, Args...>(c));
        }

        template <typename Ret, typename T1 = T>
        static me& closure(const std::string& name, std::function<Ret(T1&)> c) {
            return func(name, new tbridgeClosure<Ret, T1, tmarshaling>(c));
        }

        template <typename Ret, typename... Args>
        static me& func(const std::string& name, Ret (T::*fptr)(Args...)) {
            return funcNonConst(name, fptr);
        }

        template <typename Ret, typename... Args>
        static me& func(const std::string& name, Ret (T::*fptr)(Args...) const) {
            return funcConst(name, fptr);
        }

        template <typename Ret, typename... Args>
        static me& funcNonConst(const std::string& name, Ret (T::*fptr)(Args...)) {
            return func(name, new tbridgeFunc<Ret, T, true, tmarshaling, Args...>(fptr));
        }

        template <typename Ret, typename... Args>
        static me& funcConst(const std::string& name, Ret (T::*fptr)(Args...) const) {
            typedef typename T::super s;
            return func(name,
                new tbridgeFunc<Ret, T, true, tmarshaling, Args...>((Ret(T::*)(Args...)) fptr));
        }

        template <typename Ret, typename... Args>
        static me& genericFunc(const std::string& name, Ret (T::*fptr)(Args...)) {
            return func(name, new tbridgeFunc<Ret, T, true, tgenericMarshaling, Args...>(fptr));
        }

        template <typename Ret, typename... Args>
        static me& genericFuncNonConst(const std::string& name, Ret (T::*fptr)(Args...)) {
            return func(name,
                new tbridgeFunc<Ret, T, true, tgenericMarshaling, Args...>(
                    (Ret(T::*)(Args...)) fptr));
        }

        template <typename Ret, typename... Args>
        static me& genericFunc(const std::string& name, Ret (T::*fptr)(Args...) const) {
            return func(name,
                new tbridgeFunc<Ret, T, true, tgenericMarshaling, Args...>(
                    (Ret(T::*)(Args...)) fptr));
        }

        template <typename Ret, typename... Args>
        static me& genericFuncConst(const std::string& name, Ret (T::*fptr)(Args...) const) {
            return func(name,
                new tbridgeFunc<Ret, T, true, tgenericMarshaling, Args...>(
                    (Ret(T::*)(Args...)) fptr));
        }

    private:
        static me& _get() {
            static me _inner;
            return _inner;
        }

    private:
        scope _subs;
    };
} // namespace nm
