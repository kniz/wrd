#pragma once

#include "../func.hpp"
#include "../scope.hpp"
#include "../exprs/blockExpr.hpp"

namespace wrd {

    class _wout mgdFunc : public func {
        WRD(CLASS(mgdFunc, func),
            FRIEND_VERIFY(mgdFunc, subNodes))

    public:
        explicit mgdFunc(const params& ps, const wtype& retType);
        explicit mgdFunc(const params& ps, const wtype& retType, const blockExpr& newBlock);

    public:
        blockExpr& getBlock();
        const blockExpr& getBlock() const;
        const wtype& getRetType() const override;
        nbicontainer& subs() override;
        const params& getParams() const override;

        using super::run;
        str run(const ucontainable& args) override;

    private:
        wbool _inFrame(scope& args);
        void _outFrame();
        scope* _evalArgs(const ucontainable& args);

    private:
        params _params;
        nmap _shares;
        const wtype* _retType;
        tstr<blockExpr> _blk;
    };
}
