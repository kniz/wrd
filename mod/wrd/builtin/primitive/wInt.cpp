#include "wInt.hpp"
#include "../../ast/param.hpp"
#include "../../ast/defaultMakeCtor.hpp"
#include "../../ast/defaultMakeCopyCtor.hpp"
#include "wFlt.hpp"
#include "wChar.hpp"
#include "wBool.hpp"

namespace wrd {

    WRD_DEF_ME(wInt)

    dumScope* me::_onMakeCtors() const {
        static wInt inner;
        scope scapegoat;
        scapegoat.add(baseObj::CTOR_NAME, new defaultMakeCtor(inner));
        scapegoat.add(baseObj::CTOR_NAME, new defaultMakeCopyCtor(inner));
        return new dumScope(scapegoat);
    }

    wbool me::wIntType::isImmutable() const { return true; }

    const ases& me::wIntType::_getImpliAses() const {
        static ases inner;
        if(inner.len() <= 0) {
            inner.add(new asPrimitive<wFlt, wint>());
            inner.add(new asPrimitive<wChar, wint>());
            inner.add(new asPrimitive<wBool, wint>());
        }

        return inner;
    }

    const ases& me::wIntType::_getAses() const {
        static ases inner;
        if(inner.len() <= 0) {
            struct asStr : public tas<wStr> {
                str as(const node& me, const type& to) const override {
                    return str(new wStr(std::to_string(me.cast<wint>())));
                }
            };
            inner.add(new asStr());
        }

        return inner;
    }

    me::wInt() {}
    me::wInt(wint val): super(val) {}
}
