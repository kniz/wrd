#pragma once

#include "ctor.hpp"

namespace namu {

    class visitor;

    class _nout defaultCopyCtor : public ctor {
        NAMU(CLASS(defaultCopyCtor, ctor), VISIT())

    public:
        defaultCopyCtor(const node& org);

    public:
        using super::run;
        str run(const args& a) override;

        const params& getParams() const override;

    public:
        params _params;
    };
}
