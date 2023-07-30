#pragma once

#include "tnmap.hpp"
#include "../../../ast/node.hpp"

namespace namu {

    class node;
    template <typename K, typename V, typename defaultContainer = tnmap<K, V>>
    class tnchain : public tnbicontainer<K, V> {
        typedef tnbicontainer<K, V> _super_;
        typedef tnchain<K, V, defaultContainer> _me_;

        NAMU(CLASS(_me_, _super_))

    public:
        typedef typename super::iter iter;
        typedef typename super::iteration iteration;
        friend class chainIteration;
#include "../iter/nchainIteration.hpp"

    public:
        tnchain();
        explicit tnchain(const super& arr): _map(arr) {}
        explicit tnchain(const super* arr): _map(arr) {}

    public:
        // has:
        nbool has(const K& key) const override;

        // len:
        ncnt len() const override;
        ncnt chainLen() const;

        using super::get;
        V& get(const K& key) override;

        // add:
        using super::add;
        nbool add(const K& key, const V& val) override;

        // link:
        tstr<me> link(const super& new1);
        virtual nbool link(const me& new1);
        nbool unlink();

        // del:
        using super::del;
        nbool del(const K& at) override;
        nbool del(const iter& at) override;
        nbool del(const iter& from, const iter& end) override;

        // etc:
        void rel() override;

        super& getContainer();
        const super& getContainer() const {
            return *_map;
        }

        me& getNext() {
            return *_next;
        }
        const me& getNext() const {
            return *_next;
        }

        /// returned deep cloned of this object.
        /// @remark even if the chain has already linked to the another chain instance,
        ///         only this object will be deep cloned. cloned instance has the same linkage like
        ///         which the original chain object has.
        clonable* deepClone() const override {
            me* ret = wrap(((super*) getContainer().deepClone())->template cast<super>());
            ret->link(getNext());
            return ret;
        }

        /// wrap given container as the same level to this chain.
        /// @param toShallowWrap
        ///        if this is a chain, then the wrap func returns it as it is.
        ///        if this is any container except chain, then it returns after
        ///        wrapping given container.
        static me* wrap(const super& toShallowWrap);
        static me* wrap(const super* toShallowWrap) {
            return wrap(*toShallowWrap);
        }

        /// wrap given container no matter what it is.
        static me* wrapDeep(const super& toDeepWrap) {
            me* innerChn = wrap(toDeepWrap);

            me* ret = new me();
            ret->_map.bind(innerChn);
            return ret;
        }

        static me* wrapDeep(const super* toDeepWrap) {
            return wrapDeep(*toDeepWrap);
        }

    protected:
        iteration* _onMakeIteration(ncnt step) const override {
            // TODO: optimize using containerIteration
            me* unconst = const_cast<me*>(this);
            iteration* ret = new chainIteration(*unconst, _map->begin());
            ret->next(step);
            return ret;
        }
        iteration* _onMakeIteration(const K& key) const override {
            me* unconst = const_cast<me*>(this);
            return new chainIteration(*unconst, _map->iterate(key), key);
        }

        void _getAll(const K& key, narr& tray) const override;

    private:
        iter& _getMapIterFromChainIter(const iter& wrapper) {
            if(!wrapper._step->getType().template isSub<chainIteration>()) return nulOf<iter>();
            chainIteration& cast = (chainIteration&) *wrapper._step;
            if(nul(cast)) return nulOf<iter>();

            return cast._iter;
        }

    private:
        tstr<super> _map;
        tstr<me> _next;
    };

    typedef tnchain<std::string, node> nchain;
}
