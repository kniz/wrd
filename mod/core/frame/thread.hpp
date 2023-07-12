#pragma once

#include "../builtin/container/mgd/tarr.hpp"
#include "../builtin/container/native/tnmap.hpp"
#include "frame.hpp"

struct exprTest;

namespace namu {

    class slotLoader;
    typedef tnarr<frame> frames;

    class _nout thread : public node {
        NAMU(CLASS(thread, node))
        friend class verifier; // for frames
        friend class baseObj; // for frames.
        friend class mgdFunc; // for frames.
        friend class blockExpr; // for frames.
        friend class defVarExpr; // for frames
        friend class defAssignExpr;
        friend class assignExpr;
        friend class verifier;
        friend class retExpr;
        friend class runExpr;
        friend class func; // for frames.
        friend class forExpr;
        friend class whileExpr;
        friend class retStateExpr;
        friend class starter;
        friend struct ::exprTest;

    private:
        thread();
        thread(const node& root);

    public:
        const frames& getFrames() const NAMU_UNCONST_FUNC(_getFrames())
        const frame& getNowFrame() const NAMU_UNCONST_FUNC(_getNowFrame())

        static thread& get();
        static const instancer& getInstancer();

        // node:
        nbicontainer& subs() override;

        nbool canRun(const args& a) const override;
        str run(const args& a) override;

        void rel() override;

        /// @return slot instances loaded by internal system.
        ///         you can cast these to 'slot' types.
        const nmap& getSlots() const;

    protected:
        frames& _getFrames();
        frame& _getNowFrame();

    private:
        static thread** _get();
        void _loadBuiltIns(nmap& tray) const;

    private:
        frames _frames;
        str _root;
    };

}
