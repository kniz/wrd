#pragma once

#include "../expr.hpp"
#include "../param.hpp"
#include "../scope.hpp"

namespace namu {

    class visitor;

    class _nout assignExpr : public expr {
        NAMU(CLASS(assignExpr, expr, expr::exprType), VISIT())
        friend class verifier;
        friend class mgdFunc;
        typedef scopes::iter iter;

    public:
        assignExpr(const node& lhs, const node& rhs);

    public:
        using super::run;
        str run(const args& a) override;
        str getEval() const override;
        const node& getLeft() const;
        const node& getRight() const;
        clonable* deepClone() const override;

    private:
        iter _getScopeIterOfLhs();

    private:
        str _lhs; // name of variable or lambda.
        str _rhs;
    };
}
