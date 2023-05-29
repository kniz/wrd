#pragma once

#include "id.hpp"
#include "clonable.hpp"
#include <map>

struct namuTest;

namespace namu {

    class bindTag;
    class instancer;

    class _nout instance : public typeProvidable, public clonable {
        NAMU_DECL_ME(instance)
        NAMU_INIT_META(me)
        friend class bindTag;
        friend class watcher; // for vault.
        friend class chunks; // for vault.
        friend class instancer; // for _id.
        friend struct ::namuTest; // for vault.

    public:
        class vault {

        public:
            void set(void* ptr, nidx chkN);
            nidx get(void* ptr);
            ncnt len() const;
            nbool rel();
            std::map<void*, int>& getVaults();

        private:
            std::map<void*, int> _vaults;
        };

        //  instance:
        instance();
        explicit instance(id newId);
        instance(const me& rhs);
        virtual ~instance();

    public:
        void* operator new(size_t sz);
        void operator delete(void* pt, size_t sz);

    public:
        virtual id getId() const;
        virtual nbool isHeap() const;
        const bindTag& getBindTag() const;

    protected:
        //  instance:
        nbool _setId(id new1);
        static instancer& _getMgr();

    private:
        id _id;
        static vault _vault;
    };
}
