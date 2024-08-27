#pragma once

#include "baseObj.hpp"
#include "scope.hpp"
#include "statable.hpp"

namespace nm {

    class mgdType;
    class origin;
    class runExpr;

    /// obj is a object structured in managed programming environment like 'namu'.
    /// owned sub nodes of a object can only be manipulated through API provided obj.
    /// because native object won't have owned nodes but only shared ones.
    class _nout obj: public baseObj {
        NM(ME(obj, baseObj), INIT_META(obj), CLONE(obj), VISIT())

    public:
        typedef ntype metaType;
        friend class slot;

    public:
        explicit obj(); // this means 'any' class.
        explicit obj(const me& rhs);

    protected:
        explicit obj(scope& shares, scope& owns);

    public:
        me& operator=(const me& rhs);

    public:
        using super::subs;
        scope& subs() override;

        tstr<nbicontainer> mySubs() const override;

        state getState() const override;

        scope& getShares();
        const scope& getShares() const NM_CONST_FUNC(getShares())
        scope::super& getOwns();
        const scope::super& getOwns() const NM_CONST_FUNC(getOwns())

        const ntype& getType() const override;
        nbool isComplete() const override;

        clonable* cloneDeep() const override;

        virtual node& getCallComplete();
        const node& getCallComplete() const NM_CONST_FUNC(getCallComplete())

    protected:
        void _inFrame(frame& fr, const bicontainable& args) override;

    private:
        me& _assign(const me& rhs);

    private:
        tstr<scope> _subs;
    };

#ifdef NM_BUILD_PLATFORM_IS_WINDOWS
    // f***ing annoying another MSVC bug here:
    //  first of all, I'm so sorry to start my slang. but I couldn't help spitting it out after
    //  I used plenty hours of heading to the ground.
    //
    //  I don't know why, but unless define below variable here, I'll observe that the
    //  member-variable
    //  '_subs' above was tried to be instantiated but failed.
    //  error message said that 'You've used undefined type "identifiertstr<scope>"'.
    //  however, MSVC definately knows about tstr<T> and scope types.
    //
    //  clang, gcc didn't complain about this.
    namespace {
        static const inline scope a3;
    }
#endif
} // namespace nm
