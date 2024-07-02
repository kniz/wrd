#pragma once

#include "../interface/instance.hpp"
#include "../binder/tbindable.hpp"

namespace nm {

    class chunk;
    class _nout bindTag : public typeProvidable, public tbindable<instance> {
        NM_ME(bindTag)
        NM_INIT_META(me)
        template <typename T, typename TACTIC> friend class tweak;
        template <typename T, typename TACTIC> friend class tstr;
        friend class weakTactic;
        friend class strTactic;

    public:
        //  bindTag:
        bindTag();
        explicit bindTag(id newId);
        ~bindTag();

    public:
        instance* operator->();
        instance& operator*();
        const instance* operator->() const NM_CONST_FUNC(operator->())
        const instance& operator*() const NM_CONST_FUNC(operator*())

    public:
        const chunk& getChunk() const;
        ncnt getStrongCnt() const;
        //  tbindable:
        void rel() override;
        nbool isBind() const override;
        const type& getBindable() const;
        using tbindable::canBind;

        instance& get();
        const instance& get() const NM_CONST_FUNC(get())
        template <typename E>
        E& get() {
            instance& got = get();
            if(nul(got)) return nulOf<E>();

            return got.template cast<E>();
        }
        template <typename E>
        const E& get() const NM_CONST_FUNC(get<E>())

        nbool canBind(const type& cls) const override;
        nbool bind(const instance& new1) override;
        //  Instance:
        id getId() const;
        //  typeProvidable:
        const type& getType() const override;
        static const bindTag& getBindTag(id newId);

    private:
        //  bindTag:
        nbool _onStrong(ncnt vote);
        nbool _completeId(instance& it);
        nbool _sync(id new1);

    private:
        instance* _pt;
        ncnt _strong;
        id _id;
    };
}
