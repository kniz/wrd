#include "wChar.hpp"
#include "../../ast/defaultCtor.hpp"
#include "../../ast/defaultCopyCtor.hpp"
#include "wInt.hpp"
#include "wFlt.hpp"
#include "wBool.hpp"

namespace wrd {

    WRD_DEF_ME(wChar)

    wbool me::wCharType::isImmutable() const { return true; }

    const ases& me::wCharType::_getImpliAses() const {
        static ases inner;
        if(inner.len() <= 0) {
            inner.add(new asPrimitive<wInt, wchar>());
            inner.add(new asPrimitive<wFlt, wchar>());
            inner.add(new asPrimitive<wBool, wchar>());
        }

        return inner;
    }

    const ases& me::wCharType::_getAses() const {
        static ases inner;
        if(inner.len() <= 0) {
            struct asStr : public tas<wStr> {
                str as(const node& me, const type& to) const override {
                    std::string val;
                    val.push_back(me.cast<wchar>());
                    return str(new wStr(val));
                }
            };
            inner.add(new asStr());
        }

        return inner;
    }

    me::wChar() {}
    me::wChar(wchar val): super(val) {}

    dumScope* me::_onMakeCtors() const {
        scope scapegoat;
        scapegoat.add(baseObj::CTOR_NAME, new defaultCtor(getType()));
        scapegoat.add(baseObj::CTOR_NAME, new defaultCopyCtor(getType()));
        return new dumScope(scapegoat);
    }
}
