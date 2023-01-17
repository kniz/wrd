#pragma once

#include "../expr.hpp"
#include "../param.hpp"
#include "../../builtin/primitive/arithmeticObj.hpp"
#include "../scope.hpp"

namespace namu {

    class visitor;

    // FAO: Fundermental Arithmetic Operation.
    class _nout FAOExpr : public expr {
        NAMU(CLASS(FAOExpr, expr, expr::exprType), VISIT())

    public:
        enum Rule {
            ADD = 0,
            START = ADD,
            SUB,
            MUL,
            DIV,
            MOD,
            EQ,
            NE,
            GT,
            LT,
            GE,
            LE,
            END
        };

    public:
        FAOExpr(Rule rule, const node& lhs, const node& rhs): mRule(rule), _lhs(str(lhs)), _rhs(str(rhs)) {}

    public:
        using super::run;
        str run(const args& a) override;

        const node& getEval() const override;
        const node& getLeft() const { return *_lhs; }
        void setLeft(const node& new1) { _lhs.bind(new1); }
        const node& getRight() const { return *_rhs; }
        void setRight(const node& new1) { _rhs.bind(new1); }

    private:
        Rule mRule;
        str _lhs;
        str _rhs;
    };
}
