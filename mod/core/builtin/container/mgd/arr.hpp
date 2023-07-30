#pragma once

#include "../../../bridge/cpp/tgenericCppBridge.hpp"

namespace namu {

    // another f**king annoying MSVC Hack:
    //  to avoid C2901 error, I need to declare sort of dllexport(import) things at here.
    //  spended plenty of hours again to find out the reason. thank you so much.
    typedef class _nout tgenericCppBridge<narr> __arrSuperClass;

    class _nout arr : public __arrSuperClass, public tucontainable<node>, tarrayable<node> {
        // arr uses instance variable 'ntype':
        //  ntype contains beanType as 'const type*' instance variable. so user should be
        //  careful when calling ttype<arr>. because it will also return ntype instance
        //  as a consequences, but it won't contain beanType.
        //
        //  the most appropriate getter for ntype of arr is to call getType() of instance
        //  to arr.
        NAMU(ME(arr, __arrSuperClass),
             INIT_META(arr),
             CLONE(arr),
             VISIT())
        typedef ntype metaType; // for ttype<T>
        typedef typename tucontainable<node>::iter iter;
        typedef typename tucontainable<node>::iteration iteration;
        typedef std::map<const type*, tstr<scope>> cache;
        friend class starter; // for _cache

    public:
        arr();
        arr(const node& elemType);

    public:
        using tarrayable<node>::operator[];
        node& operator[](nidx n) override;

    public:
        const ntype& getType() const override;

        using super::subs;
        nbicontainer& subs() override;

        ncnt len() const override;

        nbool has(nidx n) const override;

        //  get:
        using tarrayable<node>::get;
        using super::get;
        template <typename E>
        E& get(std::function<nbool(const E&)> l) const {
            for(const node& elem : *this) {
                const E& cast = elem.template cast<E>();
                if(!nul(cast) && l(cast)) // elem should be typeProvidable.
                    return (E&) cast;
            }

            return nulOf<E>();
        }

        node& get(std::function<nbool(const node&)> l) const;

        template <typename E>
        tnarr<E, strTactic> getAll(std::function<nbool(const E&)> l) const {
            tnarr<E> ret;
            for(const node& elem : *this) {
                const E& cast = elem.template cast<E>();
                if(!nul(cast) && l(cast))
                    ret.add(cast);
            }

            return ret;
        }

        narr getAll(std::function<nbool(const node&)> l) const;

        node& get(nidx n) override;

        //  set:
        using tucontainable<node>::set;
        using tarrayable<node>::set;
        nbool set(const iter& at, const node& new1) override;
        nbool set(nidx n, const node& new1) override;

        //  add:
        using tucontainable<node>::add;
        using tarrayable<node>::add;
        nbool add(const iter& at, const node& new1) override;
        nbool add(nidx n, const node& new1) override;
        void add(const iter& here, const iter& from, const iter& to) override;

        //  del:
        using tucontainable<node>::del;
        nbool del(nidx n) override;
        nbool del(const iter& it) override;
        nbool del(const iter& from, const iter& to) override;

        //  etc:
        clonable* deepClone() const override;
        void rel() override;

        const obj& getOrigin() const override;

        std::string asStr() const;

    protected:
        iteration* _onMakeIteration(ncnt step) const override;

    private:
        scope& _defGeneric(const type* key);
        scope& _getOriginScope();

    private:
        static inline cache _cache;
        mutable tstr<obj> _org;
        ttype<arr> _type;
    };
}
