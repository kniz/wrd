#pragma once

#include "tcppBridgeFunc.hpp"
#include "marshaling/tgenericMarshaling.hpp"
#include "../../ast/obj.hpp"
#include "../../type/mgdType.hpp"
#include "../../ast/defaultCopyCtor.hpp"
#include "../../ast/defaultCtor.hpp"

namespace namu {

    /// bridge object only can shares 'shared' sub nodes.
    /// @param T represents native class.
    /// @param S represents whether it's generic obj or not.
    template <typename T, typename S = obj>
    class tcppBridge : public S {
        // TODO: how to impement 'as()' on bridge obj:
        //  each tcppBridge obj has its unique type object. when it got called 'getType()'
        //  it returns its type object.
        //
        //  however, type object is dynamically belongs to this bridge object, when user
        //  tries to get ttype<T>, it's not derived from ntype so it won't have any 'as()'
        //  func. user can't operate conversion in this way.
        NAMU(CLASS(tcppBridge, S))
        template <typename Ret, typename T1, typename S1, template <typename, typename, nbool> class Marshaling, typename...Args>
        friend class tcppBridgeFunc;

    public:
        tcppBridge(): super(new mgdType(ttype<T>::get().getName())), _real(nullptr) {
            _subs.bind(new scope());
        }
        tcppBridge(T* real): super(new mgdType(ttype<T>::get().getName())), _real(real) {
            _subs.bind(new scope());
        }
        tcppBridge(const me& rhs): super(rhs) {
            _real = nul(rhs._real) ? nullptr : new T(*rhs._real);
        }

    public:
        static me& def() {
            me* ret = new me(new T());
            // TODO: need to handle ctor with argument properly.
            ret->subs().add(baseObj::CTOR_NAME, new defaultCtor(*ret));
            ret->subs().add(baseObj::CTOR_NAME, new defaultCopyCtor(*ret));
            return *ret;
        }

        using super::subs;
        nbicontainer& subs() override { return *_subs; }

        template <typename Ret, typename... Args>
        me& func(const std::string& name, Ret(T::*fptr)(Args...)) {
            subs().add(name, new tcppBridgeFunc<Ret, T, S, tmarshaling, Args...>(fptr));
            return *this;
        }
        template <typename Ret, typename... Args>
        me& func(const std::string& name, Ret(T::*fptr)(Args...) const) {
            subs().add(name, new tcppBridgeFunc<Ret, T, S, tmarshaling, Args...>( (Ret(T::*)(Args...)) fptr));
            return *this;
        }

        T& get() {
            return *_real;
        }
        const T& get() const {
            return *_real;
        }

        const obj& getOrigin() const override {
            // if an object doesn't have owned sub nodes it means that all instances of that classes
            // are same and origin simulteneously.
            return *this;
        }

        // TODO: add defaultCtor

    private:
        T* _real;
        tstr<scope> _subs;
    };
}
