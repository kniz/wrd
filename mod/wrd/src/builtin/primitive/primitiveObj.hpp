#pragma once

#include "../../ast/mgd.hpp"

namespace wrd {

    template <typename T>
    class primitiveObj : public obj {
        WRD(INTERFACE(primitiveObj, obj))

    public:
        primitiveObj(): _val() {}
        primitiveObj(const T& val): _val(val) {}

        T& get() { return _val; }
        const T& get() const { return _val; }

        using super::getCtors;
        funcs& getCtors() override {
            static funcs* ctors = nullptr;
            if(!ctors) {
                ctors = new funcs();
                _onCreateCtors(*ctors);
            }

            return *ctors;
        }
        const obj& getOrigin() const override {
            return *this;
        }

    protected:
        wbool _onSame(const typeProvidable& rhs) const override {
            const me& cast = (const me&) rhs;
            return _val == cast._val;
        }

        virtual void _onCreateCtors(funcs& tray) const = 0;

    private:
        T _val;
    };
}
