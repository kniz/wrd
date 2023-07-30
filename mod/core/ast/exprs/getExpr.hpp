#pragma once

#include "../expr.hpp"
#include "../params.hpp"

namespace namu {

    class visitor;

    class _nout getExpr : public expr {
        NAMU(CLASS(getExpr, expr, expr::exprType), VISIT())
        friend class verifier;
        friend class loweventer;
        friend class visitor; // for _me
        friend class generalizer; // for _args

    public:
        getExpr(const std::string& name);
        getExpr(const std::string& name, const args& a);
        getExpr(const node& me, const std::string& name);
        getExpr(const node& me, const std::string& name, const args& a);

    public:
        using super::run;
        str run(const args& a) override;
        str getEval() const override;
        const node& getMe() const;
        const std::string& getSubName() const;
        void setMe(const node& newMe);

        /// @return nullable
        const args& getSubArgs() const;

        clonable* deepClone() const override;

    protected:
        virtual str _get(nbool evalMode) const;

    private:
        str _me;
        std::string _name;
        tstr<args> _args;
    };
}
