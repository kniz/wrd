#pragma once

#include "../flag.hpp"

namespace namu {

    class logStructureFlag : public flag {
        NAMU(CLASS(logStructureFlag, flag))

    public:
        const nchar* getName() const override;
        const nchar* getDescription() const override;

    protected:
        const nchar* _getRegExpr() const override;
        nbool _onTake(const flagArgs& tray, cli& c, interpreter& ip) const override;
    };
}
