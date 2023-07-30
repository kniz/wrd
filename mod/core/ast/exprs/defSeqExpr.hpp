#pragma once

#include "../expr.hpp"

namespace namu {

    class visitor;

    class _nout defSeqExpr : public expr {
        NAMU(CLASS(defSeqExpr, expr, expr::exprType), VISIT())
        friend class mgdFunc;
        friend class verifier;

    public:
        defSeqExpr(const node& start, const node& end);

    public:
        using super::run;
        str run(const args& a) override;

        str getEval() const override;

        const node& getStart() const;
        const node& getEnd() const;

    private:
        str _start;
        str _end;
    };
}
