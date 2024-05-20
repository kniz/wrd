#pragma once

#include "../loader/err.hpp"

namespace namu {

    typedef err*(*errMaker)(nint);
    typedef std::function<void(const err&)> sigHandler;

    struct _nout sig {
        NAMU(ME(sig))

    public:
        nbool operator==(const sig& rhs) const;
        nbool operator!=(const sig& rhs) const;

    public:
        int code;
        errMaker maker;
    };

    class _nout signaler {
        NAMU(ME(signaler))

    private:
        signaler();

    public:
        void addSignal(sigHandler closure);
        void delSignal(sigHandler closure);
        void relSignal();

        /// report that signal has occured. and terminate the process after reporting.
        void onSignal(nint code);

        static signaler& singletone();

    private:
        void _setSignal(void(*csignalHandler)(nint));
        const err* _getErrBy(nint code) const;

    private:
        std::vector<sigHandler> _closures;
    };
}
