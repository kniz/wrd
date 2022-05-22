#pragma once

#include "../expr.hpp"
#include "../params.hpp"

namespace wrd {

    class _wout getExpr : public expr {
        WRD(CLASS(getExpr, expr, expr::exprType),
            FRIEND_VERIFY(getExpr, isRunnable))

    public:
        getExpr(const std::string& name): _name(name) {}
        getExpr(const std::string& name, const narr& args): _name(name), _args(args) {}
        getExpr(const node& from, const std::string& name): _from(from), _name(name) {}
        getExpr(const node& from, const std::string& name, const narr& args): _from(from), _name(name), _args(args) {}

    public:
        using super::run;
        str run(const ucontainable& args) override {
            // believe that this expression was checked to be valid.
            return str(_get());
        }

        const wtype& getEvalType() const override;
        const node& getFrom() const;
        const std::string& getSubName() const { return _name; }

        /// @return nullable
        const narr& getSubArgs() const { return *_args; }

    private:
        node& _get() const {
            str evalMe = getFrom().isSub<expr>() ? getFrom().as<node>() : getFrom();
            if(!evalMe) return WRD_E("from == null"), nulOf<node>();
            WRD_DI("_name=%s", _name.c_str());
            if(!_args) return evalMe->sub(_name);

            return evalMe->sub(_name, *_args);
        }

    private:
        str _from;
        std::string _name;
        tstr<narr> _args;
    };
}
