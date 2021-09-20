#pragma once

#include "../expr.hpp"
#include "../ref.hpp"

namespace wrd {

    class getExpr : public expr {
        WRD(CLASS(getExpr, expr))

    public:
        getExpr(node* from, const std::string& name, narr* args = nullptr)
            : _from(from), _name(name), _args(args) {}

    public:
        str run(const containable& args) override {
            // believe that this expression was checked to be valid.
            return str(_from->sub(_name, *_args));
        }

        const wtype& getEvalType() const override;

        wbool isValid() const override {
            if(!super::isValid()) return false;
            if(!_from) return false;

            narr subs = _from->subAll(_name, *_args);
            return subs.len() == 1;
        }

    private:
        ref _from;
        std::string _name;
        tstr<narr> _args;
    };
}
