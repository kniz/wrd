#pragma once

#include "../ast/baseObj.hpp"
#include "scopeStack.hpp"
#include "../builtin/res/tpair.hpp"

struct immutableTest;
struct frameTest;

namespace namu {

    class _nout retState : public instance {
        NAMU(ADT(retState, instance))
        friend class frame;

    public:
        // overrides operator==() by 'instance' class.

        virtual nbool isOverwritable(const retState& it) const = 0;
        virtual nbool isEmpty() const;

    protected:
        retState();
    };

    class _nout blkRetState : public retState {
        NAMU(CLASS(blkRetState, retState))
        friend class frame;

    public:
        blkRetState(nbool isOverwritable);

    public:
        nbool isOverwritable(const retState& it) const override;

    protected:
        blkRetState();

    private:
        nbool _isOverwritable;
    };

    class _nout blkEmptyRetState : public blkRetState {
        NAMU(CLASS(blkEmptyRetState, blkRetState))

    public:
        blkEmptyRetState();

    public:
        nbool isEmpty() const override;
    };

    class _nout funcRetState : public retState {
        NAMU(CLASS(funcRetState, retState))
        friend class frame;

    public:
        nbool isOverwritable(const retState& it) const override;

    protected:
        funcRetState();
    };

    class _nout funcEmptyRetState : public funcRetState {
        NAMU(CLASS(funcEmptyRetState, funcRetState))
        friend class frame;

    public:
        virtual nbool isEmpty() const override;

    protected:
        funcEmptyRetState();
    };

    class obj;
    class func;
    class _nout frame : public node { // TODO: may be obj, not node.
        NAMU(CLASS(frame, node), VISIT())
        friend class verifier;
        friend struct ::immutableTest;
        friend struct ::frameTest;
        friend class baseObj;

    public:
        static inline const blkEmptyRetState BLK_EMPTY;
        static inline const blkRetState BLK_RET = blkRetState(true);
        static inline const blkRetState BLK_BREAK = blkRetState(false);
        static inline const blkRetState BLK_NEXT = blkRetState(false);
        static inline const funcEmptyRetState FUNC_EMPTY;
        static inline const funcRetState FUNC_RETURN;

    public:
        frame();
        ~frame() override;

        /*/// @return returns tpair<node, itsOwner>. the owner has the container holding the node
        ///         which can run with given name & argument.
        tpair<str, str> subAndOwner(const std::string& name, const ucontainable& args) {
            scopes& chn = subs().cast<scopes>();
            if(nul(chn)) return tpair<str, str>();

            for(scopes* e=&chn; e ;e=&e->getNext()) {
                node& owner = _getOwnerFrom(*e);
                if(nul(owner)) {
                    NAMU_W("couldn't find owner from chain[%x]", &e.get());
                    continue;
                }

                node& ret = owner.sub(name, args);
                if(nul(ret)) continue;

                return tpair<str, str>(str(owner), str(ret));
            }

            return tpair<str, str>();
        }*/

        nbool pushLocal(nbicontainer* con);
        nbool pushLocal(nbicontainer& con);
        nbool pushLocal(scopes* new1);
        nbool pushLocal(scopes& new1);
        nbool pushLocal(const std::string& name, const node& n);
        nbool pushLocal(const std::string& name, const node* n);
        void pushObj(const baseObj& obj);

        nbicontainer& getTop();
        const nbicontainer& getTop() const NAMU_UNCONST_FUNC(getTop())

        // I won't provide API for poping a single node from the scope.
        tstr<scopes> popLocal();

        void pushFunc(func& new1);
        void popFunc();
        func& getFunc();
        const func& getFunc() const NAMU_UNCONST_FUNC(getFunc())
        const baseObj& getMe() const NAMU_UNCONST_FUNC(getMe())
        baseObj& getMe();

        // node:
        using node::subs;
        nbicontainer& subs() override;

        nbool canRun(const args& a) const override;
        str run(const args& a) override;

        void rel() override;

        void relRet();
        nbool setRet(const retState& new1);
        nbool setRet(const retState& new1, const node& toRet);
        const retState& getRetState() const;
        node& getRet() const;

    protected:
        static baseObj& _setMe(baseObj& new1);
        static baseObj& _setMe();
        static baseObj& _getMe();

    private:
        void _rel();

    private:
        tstr<retState> _retState;
        tstr<baseObj> _obj;
        std::vector<tstr<func>> _funcs;
        scopeStack _local;
        mutable str _ret;
    };
}
