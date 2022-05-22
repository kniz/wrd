#pragma once

#include "parser.hpp"
#include "srcSupply.hpp"
#include "verifier.hpp"

namespace wrd {

    class _wout interpreter : public typeProvidable, public clonable {
        WRD(CLASS(interpreter))

    public:
        interpreter(): _isParsed(false), _isLogStructure(false), _isLogInterpreter(false) {}

    public:
        me& setReport(errReport& report) {
            _rpt.bind(report);
            return *this;
        }
        me& setPack(pack& pak) {
            _pak.bind(pak);
            return *this;
        }
        me& setSrcSupply(const srcSupply& supply) {
            _srcs.bind(supply);
            return *this;
        }
        me& setLogStructure(wbool enable) {
            _isLogStructure = enable;
            return *this;
        }
        me& setLogInterpreter(wbool enable) {
            _isLogInterpreter = enable;
            return *this;
        }
        wbool isParsed() const {
            return _isParsed;
        }

        wbool isVerified() const {
            return isParsed() && (_rpt && !_rpt->hasErr());
        }

        node& getSubPack() { return _pser.getSubPack(); }
        const node& getSubPack() const WRD_UNCONST_FUNC(getSubPack())

        pack& getPack() { return _pser.getPack(); }
        const pack& getPack() const WRD_UNCONST_FUNC(getPack())

        const errReport& getReport() const {
            return *_rpt;
        }

        pack& interpret() {
            if(!_srcs) {
                _rpt->add(err::newErr(NO_SRC));
                return *_pak;
            }

            _parse();
            tstr<frame> info;
            if(*_rpt)
                return *_pak;
            _verify(info);
            _logStructure(*info, _srcs->get());

            return *_pak;
        }

        void rel() {
            _isParsed = false;
            _rpt.rel();
            _veri.rel();
            _pser.rel();
            _srcs.rel();
            _pak.rel();
        }

        void log() const {
            if(!_rpt && !*_rpt) return;

            logger& l = logger::get();
            l.saveStreamEnable();
            l.setEnable(true);

            _rpt->log();

            l.loadStreamEnable();
        }

    private:
        wbool _isPackExist() {
            return !nul(_pser.getSubPack()) && _pak;
        }

        void _parse() {
            logger& l = logger::get();
            l.saveStreamEnable();
            l.setEnable(_isLogInterpreter);
            while(_srcs->next()) {
                _pser.rel(); // parser can only take 1 src.

                const char* buf = _srcs->get();
                WRD_DI("======================================");
                WRD_DI("                parse                 ");
                WRD_DI("======================================");

                _pser.setReport(*_rpt)
                     .setPack(*_pak)
                     .parse(buf);

                if(!_pak)
                    _pak.bind(_pser.getPack());
            }

            _isParsed = _isPackExist() && !_rpt->hasErr();
            l.loadStreamEnable();
        }

        void _verify(tstr<frame>& info ) {
            logger& l = logger::get();
            l.saveStreamEnable();
            l.setEnable(_isLogInterpreter);

            WRD_DI("======================================");
            WRD_DI("                verify                ");
            WRD_DI("======================================");

            if(!_pak) {
                WRD_E("_pak is null");
                return;
            }

            // make tray:
            packs* paks = new packs();
            paks->add(_pak->getManifest().name, *_pak);
            packChain tray(paks);
            tray.link(thread::get().getSystemPacks());

            // verify:
            _veri.setReport(*_rpt)
                 .setPacks(tray)
                 .setFrameInfo(info)
                 .verify(*_pak);
            l.loadStreamEnable();

        }

        void _logStructure(frame& info, const wchar* buf) {
            if(!_isLogStructure) return;

            logger& l = logger::get();
            l.saveStreamEnable();
            l.setEnable(true);

            std::cout << " - frame:\n";
            _logFrame(info);
            std::cout << "\n";

            if(!nul(_pser.getSubPack()) && _pak) {
                std::cout << " - structure:\n";
                _logStructure(_pser.getSubPack(), _pak->getManifest().name, 0, 0, true, true);
                std::cout << "\n";
            }

            l.loadStreamEnable();
        }

        void _logStructure(const node& n, const std::string& name, int idx, int level, bool isLast, bool isParentLast) const {
            if(nul(n)) {
                WRD_W("_logStructure(n == null)");
                return;
            }

            _logIndent(level, isParentLast);
            std::cout << (isLast ? "┗━[" : "┣━[") << idx << "]: " << n.getType().getName() << " \"" << name << "\"\n";

            int subN = -1;
            const bicontainable& subs = n.subs();
            for(auto e=subs.begin(); e ;++e) {
                subN++;
                _logStructure(e.getVal(), e.getKey(), subN, level + 2, subN == subs.len()-1, isLast);
            }

            const mgdFunc& f = n.cast<mgdFunc>();
            if(!nul(f)) {
                subN++;
                _logStructure(f.getBlock().getStmts(), subN, level+2, subN == subs.len(), isLast);
            }
        }

        void _logStructure(const narr& blk, int idx, int level, bool isLast, bool isParentLast) const {
            _logIndent(level, isParentLast);
            std::cout << (isLast ? "┗━[" : "┣━[") << idx << "]: block \n";

            int subN = -1;
            for(const auto& stmt: blk) {
                subN++;
                const blockExpr& blkExpr = stmt.cast<blockExpr>();
                if(!nul(blkExpr))
                    _logStructure(blkExpr.getStmts(), subN, level+2, subN == blk.len()-1, isLast);
                else
                    _logStructure(stmt, "", subN, level + 2, subN == blk.len()-1, isLast);
            }
        }

        void _logIndent(int level, bool isParentLast) const {
            std::cout << "  ";
            for(int n=0; n < level-1; n++)
                std::cout << (isParentLast ? "  " : "┃ ");
        }

        void _logFrame(const frame& info) const {
            if(nul(info)) {
                std::cout << "    null\n";
                return;
            }

            int n=0;
            for(auto e = info.subs().begin(); e ;++e)
                std::cout << "    [" << n++ << "]: '" << e.getKey() << "' " << e.getVal().getType().getName().c_str() << "\n";
        }


    private:
        tstr<errReport> _rpt;
        tstr<pack> _pak;
        verifier _veri;
        parser _pser;
        tstr<srcSupply> _srcs;
        wbool _isParsed;
        wbool _isLogStructure;
        wbool _isLogInterpreter;
    };
}
