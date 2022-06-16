#pragma once

#include "primitiveObj.hpp"

namespace wrd {

    class _wout wChar : public primitiveObj<wchar> {

        class _wout wCharType : public wtype {
        public:
            wbool isImmutable() const override;

        protected:
            const ases& _getImpliAses() const override;
            const ases& _getAses() const override;
        };

        WRD(CLASS(wChar, primitiveObj, wCharType))

    public:
        wChar();
        wChar(wchar val);

    protected:
        dumScope* _onMakeCtors() const override;

        tstr<arithmeticObj> _add(const arithmeticObj& rhs) const override {
            return new me(get() + rhs.as<me>()->get());
        }
        tstr<arithmeticObj> _sub(const arithmeticObj& rhs) const override {
            return new me(get() - rhs.as<me>()->get());
        }
        tstr<arithmeticObj> _mul(const arithmeticObj& rhs) const override {
            return new me(get() * rhs.as<me>()->get());
        }
        tstr<arithmeticObj> _div(const arithmeticObj& rhs) const override {
            return new me(get() / rhs.as<me>()->get());
        }
        tstr<arithmeticObj> _mod(const arithmeticObj& rhs) const override {
            return new me(get() % rhs.as<me>()->get());
        }
    };
}
