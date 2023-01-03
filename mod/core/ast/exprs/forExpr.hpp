#pragma once

#include "../expr.hpp"
#include "blockExpr.hpp"

namespace namu {

    class frame;
    class _nout forExpr : public expr {
        NAMU(CLASS(forExpr, expr, expr::exprType), VISIT())
        friend class verifier;

    public:
        forExpr(const std::string localName, const node& container, const blockExpr& blk);

    public:
        const std::string& getLocalName() const;
        const blockExpr& getBlock() const NAMU_UNCONST_FUNC(getBlock());
        blockExpr& getBlock();
        str getContainer();

        using super::run;
        str run(const args& a) override;

        const node& getEval() const override;

    private:
        /// @return true if exit the loop
        nbool _postProcess(frame& fr);

    private:
        str _container;
        std::string _name;
        mutable tstr<blockExpr> _blk;
    };
}
