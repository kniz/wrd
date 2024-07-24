#pragma once

#include "../../ast/baseFunc.hpp"
#include "../../ast/args.hpp"
#include "../../ast/params.hpp"
#include "ast/mockNode.hpp"
#include <functional>

namespace nm {

    template <typename Ret, typename T, template <typename, nbool> class Marshaling, typename... Args>
    class tbridgeClosure : public baseFunc {
        NM(CLASS(tbridgeClosure, baseFunc))
        static_assert(allTrues<(sizeof(Marshaling<Args, tifSub<Args, node>::is>::canMarshal() ) ==
                sizeof(metaIf::yes))...>::value,
                "can't marshal one of this func's parameter ntypes.");

    public:
        tbridgeClosure(std::function<Ret(T&, Args...)>&& closure): _closure(closure) {}

    public:
        using super::run;
        str run(const args& a) override {
            args tray;
            args& evaluated = _evalArgs(a, tray);
            if(nul(evaluated))
                return NM_E("evaluated == null"), str();

            return _marshal(evaluated);
        }

        str getRet() const override {
            if(!this->_ret)
                this->_ret.bind(Marshaling<Ret, tifSub<Ret, node>::is>::onGetRet());

            return this->_ret;
        }

        const params& getParams() const override {
            if(!_params) {
                _params.bind(new params());
                (_params->add(new param("", Marshaling<Args, tifSub<Args, node>::is>::onAddParam())), ...);
            }
            return *_params;
        }

        clonable* cloneDeep() const override {
            me* ret = (me*) clone();
            const params& ps = getParams();
            if(!nul(ps))
                ret->_params.bind((params*) ps.cloneDeep());

            return ret;
        }

    private:
        template <size_t... index>
        str _marshal(args& a, std::index_sequence<index...>) {
            T* me = (T*) &a.getMe();
            if(nul(me)) return NM_E("object from frame does not exists."), str();
            if(me->template isSub<mockNode>()) {
                mockNode& mock = me->template cast<mockNode>();
                me = (T*) &mock.getTarget();
            }

            return Marshaling<Ret, tifSub<Ret, node>::is>::toMgd(
                _closure(*me, Marshaling<Args, tifSub<Args, node>::is>::toNative(a[index])...));
        }

        args& _evalArgs(const args& a, args& tray) {
            const params& ps = getParams();
            if(a.len() != ps.len())
                return NM_E("length of a(%d) and typs(%d) doesn't match.", a.len(), ps.len()),
                       nulOf<args>();

            int n = 0;
            for(const node& e: a) {
                str ased = e.as(ps[n++].getOrigin());
                if(!ased)
                    return nulOf<args>();

                tray.add(*ased);
            }
            tray.setMe(a.getMe());
            return tray;
        }


    private:
        mutable tstr<params> _params;
        std::function<Ret(Args...)> _closure;
    };

    template <typename T, template <typename, nbool> class Marshaling, typename... Args>
    class tbridgeClosure<void, T, Marshaling, Args...> : public baseFunc {
        NM(CLASS(tbridgeClosure, baseFunc))
        static_assert(allTrues<(sizeof(Marshaling<Args, tifSub<Args, node>::is>::canMarshal() ) ==
                sizeof(metaIf::yes))...>::value,
                "can't marshal one of this func's parameter ntypes.");

    public:
        tbridgeClosure(std::function<void(T&, Args...)>&& closure): _closure(closure) {}

    public:
        using super::run;
        str run(const args& a) override {
            args tray;
            args& evaluated = _evalArgs(a, tray);
            if(nul(evaluated))
                return NM_E("evaluated == null"), str();

            return _marshal(evaluated);
        }

        str getRet() const override {
            if(!this->_ret)
                this->_ret.bind(Marshaling<void, false>::onGetRet());

            return this->_ret;
        }

        const params& getParams() const override {
            if(!_params) {
                _params.bind(new params());
                (_params->add(new param("", Marshaling<Args, tifSub<Args, node>::is>::onAddParam())), ...);
            }
            return *_params;
        }

        clonable* cloneDeep() const override {
            me* ret = (me*) clone();
            const params& ps = getParams();
            if(!nul(ps))
                ret->_params.bind((params*) ps.cloneDeep());

            return ret;
        }

    private:
        template <size_t... index>
        str _marshal(args& a, std::index_sequence<index...>) {
            T* me = (T*) &a.getMe();
            if(nul(me)) return NM_E("object from frame does not exists."), str();
            if(me->template isSub<mockNode>()) {
                mockNode& mock = me->template cast<mockNode>();
                me = (T*) &mock.getTarget();
            }

            _closure(*me, Marshaling<Args, tifSub<Args, node>::is>::toNative(a[index])...);
            return Marshaling<void, tifSub<void, node>::is>::toMgd();
        }

        args& _evalArgs(const args& a, args& tray) {
            const params& ps = getParams();
            if(a.len() != ps.len())
                return NM_E("length of a(%d) and typs(%d) doesn't match.", a.len(), ps.len()),
                       nulOf<args>();

            int n = 0;
            for(const node& e: a) {
                str ased = e.as(ps[n++].getOrigin());
                if(!ased)
                    return nulOf<args>();

                tray.add(*ased);
            }
            tray.setMe(a.getMe());
            return tray;
        }


    private:
        mutable tstr<params> _params;
        std::function<void(Args...)> _closure;
    };
}
