#pragma once

#include "scope.hpp"
#include "../builtin/container/native/tnchain.hpp"

namespace namu {
    class _nout scopes : public tnchain<std::string, node, scope> {
        typedef tnchain<std::string, node, scope> _super2;
        NAMU(CLASS(scopes, _super2))

    public:
        scopes();
        explicit scopes(const super::super& arr);
        explicit scopes(const super::super* arr);
        explicit scopes(const super::super& org, const me& next): super(org, next) {}

    public:
        node& getOwner() override;
        void setOwner(const node& owner) override;
        me* cloneDeep() const override;

        using super::wrap;
        me* wrap(const super::super& toShallowWrap) const override;

    protected:
        me* _makeWhileCloneChaining(const super& rhs) const override;

    private:
        weak _owner;
    };
}
