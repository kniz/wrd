#include "arr.hpp"
#include "../../../visitor/visitor.hpp"
#include "../../../visitor/generalizer.hpp"
#include "../../../bridge/cpp.hpp"

namespace namu {

    NAMU(DEF_ME(arr), DEF_VISIT())

    namespace {
        static inline const std::string TYPENAME = "T";

        typedef tucontainable<node>::iter niter;
        typedef tcppBridge<niter> __superMgdIter;

        class _nout mgdIter : public __superMgdIter {
            NAMU(CLASS(mgdIter, __superMgdIter))

        public:
            mgdIter(niter* real): super(real) {}

        public:
            using super::subs;
            nbicontainer& subs() override {
                static super* inner = nullptr;
                if(nul(inner)) {
                    inner = new super();
                    inner->func("isEnd", &niter::isEnd)
                          .func("next", &niter::next)
                          .func<node&>("get", &niter::get);
                }

                return inner->subs();
            }
        };

        class iterateFunc : public func {
            NAMU(CLASS(iterateFunc, func))

        public:
            str getRet() const override {
                static str inner(new mgdIter(nullptr));
                return inner;
            }

            const params& getParams() const override {
                static params inner;
                if(inner.len() <= 0)
                    inner.add(new param("step", *new nInt()));

                return inner;
            }

            str run(const args& a) override {
                const params& ps = getParams();
                if(a.len() != ps.len()) return NAMU_W("a.len(%d) != ps.len(%d)", a.len(), ps.len()), str();
                arr& meObj = a.getMe().cast<arr>();
                if(nul(meObj)) return NAMU_E("meObj as arr == null"), str();

                str eval = a[0].as(ps[0].getOrigin().as<node>());
                if(!eval)
                    return NAMU_E("evaluation of arg[%s] -> param[%s] has been failed",
                            a[0].getType().getName().c_str(), ps[0].getType().getName().c_str()),
                           str();

                nint step = eval->cast<nint>();
                return new mgdIter(new niter(meObj.get().iterate(step)));
            }
        };

        class getElemTypeFunc : public func {
            NAMU(CLASS(getElemTypeFunc, func))

        public:
            getElemTypeFunc(): _ret(new getExpr(TYPENAME)) {}

        public:
            str getRet() const override {
                return _ret;
            }

            str run(const args& a) override {
                return _ret ? _ret->as<node>() : *_ret;
            }

            nbool setRet(const node& newRet) override {
                return _ret.bind(newRet);
            }

        private:
            str _ret;
        };
    }

    me::arr(): super(new narr()) { _type.getBeans().add(*new obj()); }
    me::arr(const node& newType): super(new narr()) { _type.getBeans().add(newType); }

    node& me::operator[](nidx n) {
        return get()[n];
    }

    const ntype& me::getType() const {
        return _type;
    }

    nbicontainer& me::subs() {
        const type* key = &getType().getBeans()[0].getType();
        auto e = _cache.find(key);
        if(e != _cache.end())
            return e->second.get();

        // this is first try to generalize type T:
        return _defGeneric(key);
    }

    ncnt me::len() const {
        return get().len();
    }

    nbool me::has(nidx n) const {
        return get().has(n);
    }

    node& me::get(nidx n) {
        return get().get(n);
    }

    node& me::get(std::function<nbool(const node&)> l) const {
        return this->get<node>(l);
    }

    narr me::getAll(std::function<nbool(const node&)> l) const {
        return this->getAll<node>(l);
    }

    nbool me::set(const iter& at, const node& new1) {
        if(nul(new1)) return false;
        str ased = new1.asImpli(getType().getBeans()[0]);
        if(!ased || ased->isSub<nVoid>()) return false;

        return get().set(at, *ased);
    }
    nbool me::set(nidx n, const node& new1) {
        if(nul(new1)) return false;
        str ased = new1.asImpli(getType().getBeans()[0]);
        if(!ased || ased->isSub<nVoid>()) return false;
        return get().set(n, *ased);
    }

    nbool me::add(const iter& at, const node& new1) {
        if(nul(new1)) return false;
        str ased = new1.asImpli(getType().getBeans()[0]);
        if(!ased || ased->isSub<nVoid>()) return false;

        return get().add(at, *ased);
    }

    nbool me::add(nidx n, const node& new1) {
        if(nul(new1)) return false;
        str ased = new1.asImpli(getType().getBeans()[0]);
        if(!ased || ased->isSub<nVoid>()) return false;

        return get().add(n, *ased);
    }

    void me::add(const iter& here, const iter& from, const iter& to) {
        get().add(here, from, to);
    }

    //  del:
    nbool me::del(nidx n) {
        return get().del(n);
    }

    nbool me::del(const iter& it) {
        return get().del(it);
    }

    nbool me::del(const iter& from, const iter& to) {
        return get().del(from, to);
    }

    //  etc:
    clonable* me::deepClone() const {
        me* ret = (me*) clone();
        ret->rel();
        tstr<tnarr<node>> cloned((tnarr<node>*) get().deepClone());
        ret->get().add(*cloned);
        return ret;
    }

    void me::rel() {
       get().rel();
    }

    const obj& me::getOrigin() const {
        if(!_org)
            _org.bind(new me(getType().getBeans()[0]));
        return *_org;
    }

    std::string me::asStr() const {
        return get().asStr();
    }

    me::iteration* me::_onMakeIteration(ncnt step) const {
        return get()._onMakeIteration(step);
    }

    scope& me::_defGeneric(const type* key) {
        tstr<scope> clone = _getOriginScope().deepClone();
        clone->add("getElemType", new getElemTypeFunc());

        _cache.insert({key, clone}); // this avoids infinite loop.

        generalizer g;
        g.add(*new param(TYPENAME, getType().getBeans()[0]))
         .setRoot(*this)
         .start();

        return *clone;
    }

    scope& me::_getOriginScope() {
        static super* inner = nullptr;
        if(nul(inner)) {
            inner = &super::def();
            inner->genericFunc("len", &narr::len)
                  .genericFunc("rel", &narr::rel)
                  .genericFunc<nbool, nidx>("del", &narr::del)
                  .genericFunc<nbool, const node&>("add", &tucontainable<node>::add)
                  .genericFunc<nbool, nidx, const node&>("add", &narr::add)
                  .genericFunc<nbool, nidx, const node&>("set", &narr::set)
                  .genericFunc<node&, nidx>("get", &narr::get)
                  .genericFunc("has", &narr::has);
            inner->subs().add("iterate", new iterateFunc());
        }

        return inner->subs().cast<scope>();
    }
}
