#include "ttypeBase.inl"

namespace namu {

#define ME ttypeBase<void, type>
#define SUPER type

    ME::ttypeBase() { this->init(); }

    nbool ME::isTemplate() const {
        return false;
    }

    nbool ME::isAbstract() const {
        return true;
    }

    const std::string& ME::getName() const {
        static const std::string inner = "void";
        return inner;
    }

    void* ME::make() const {
        return nullptr;
    }

    ncnt ME::size() const {
        return 0;
    }

    const type& ME::getSuper() const {
        return ttype<typename tadaptiveSuper<void>::super>::get();
    }

    const nbool& ME::isInit() const {
        static nbool inner = typeid(void) == typeid(adam);
        return inner;
    }

    const ME& ME::get() {
        static ME* inner = nullptr;
        if(!inner) {
            inner = new ME(false);
            inner->init();
        }

        return *inner;
    }

    types& ME::_getSupers() {
        static types inner;
        return inner;
    }

    types& ME::_getSubs() {
        static types inner;
        return inner;
    }

    type& ME::_getStatic() const {
        return const_cast<ME&>(get());
    }

    ME::ttypeBase(nbool) {}

    types** ME::_onGetLeafs() const {
        static types* inner = nullptr;
        return &inner;
    }

    void ME::_onAddSubClass(const type& subClass) {
        super::_onAddSubClass(subClass);

        types** leafs = _onGetLeafs();
        if(*leafs)
            this->_setLeafs(nullptr);
    }

#undef TEMPL
#undef ME
#undef SUPER
}
