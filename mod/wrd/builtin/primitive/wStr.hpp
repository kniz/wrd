#pragma once

#include "primitiveObj.hpp"

namespace wrd {

    class _wout wStr : public primitiveObj<std::string> {

		class _wout wStrType : public wtype {
		public:
			wbool isImmutable() const override { return true; }

		protected:
            const ases& _getImpliAses() const override;
		};
        WRD(CLASS(wStr, primitiveObj, wStrType))

    public:
        wStr() {}
        wStr(const wchar* val): super(std::string(val)) {}
        wStr(const std::string& val): super(val) {}

    protected:
        void _onCreateCtors(funcs& tray) const override {}
    };
}
