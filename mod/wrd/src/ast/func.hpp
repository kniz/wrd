#pragma once

#include "node.hpp"

namespace wrd {

    class func : public node {
        WRD(INTERFACE(func, node))

    public:
        explicit func(const std::string& name): _name(name) {}

        using super::subs;
        ncontainer& subs() override {
            static narr empty;
            return empty;
        }

        using super::run;
        str run(const containable& args) override;

        using super::canRun;
        wbool canRun(const wtypes& typs) const override;

        /// @return object and parameter types.
        virtual const wtypes& getTypes() const;
        const std::string& getName() const override {
            return _name;
        }

    protected:
        virtual str _onCast(narr& castedArgs) = 0;

    private:
        tstr<narr> _asArgs(const containable& args);

    private:
        std::string _name;
    };

    typedef tnarr<func> funcs;
}
