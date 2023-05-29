#pragma once

#include "type.hpp"
#include "../rtti.hpp"

namespace namu {

    /// @remark ttypeBase returns ttypeBase<type> as its meta class.
    ///         however, this makes impossible to get specific ttypeBase instance
    ///         at a binder.
    ///
    ///         if user typedefs SuperType at their type T, then ttypeBase
    ///         inherits from given SuperType.
    ///         this eventually make user add API to want to ttypeBase class.
    template <typename T, typename S = typename tmetaTypeDef<T>::is>
    class ttypeBase : public S {
        typedef ttypeBase<T, S> _T;
        NAMU_DECL_ME(_T, S)

    public:
        ttypeBase();

    public:
        nbool isTemplate() const override;
        nbool isAbstract() const override;
        const std::string& getName() const override;
        void* make() const override;
        ncnt size() const override;
        const type& getSuper() const override;
        const nbool& isInit() const override;
        static const me& get();

    protected:
        types& _getSupers() override;
        types& _getSubs() override;
        type& _getStatic() const override;
        types** _onGetLeafs() const override;
        void _onAddSubClass(const type& subClass) override;

    private:
        ttypeBase(nbool); // for skipping recursive static variable init.
    };


    template <>
    class ttypeBase<void, type> : public type {
        typedef ttypeBase<void, type> _T;
        NAMU_DECL_ME(_T, type)

    public:
        ttypeBase();

    public:
        nbool isTemplate() const override;
        nbool isAbstract() const override;

        const std::string& getName() const override;

        void* make() const override;
        ncnt size() const override;
        const type& getSuper() const override;

        const nbool& isInit() const override;

        static const me& get();

    protected:
        types& _getSupers() override;
        types& _getSubs() override;
        type& _getStatic() const override;
        types** _onGetLeafs() const override;
        void _onAddSubClass(const type& subClass) override;

    private:
        ttypeBase(nbool);
    };
}
