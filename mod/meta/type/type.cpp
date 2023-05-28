#include "type.hpp"

namespace namu {
    NAMU_DEF_ME(type)

    nbool me::operator==(const me& rhs) const {
        return getName() == rhs.getName();
    }

    nbool me::operator!=(const me& rhs) const { return !operator==(rhs); }

    const types& me::getLeafs() const {
        types* leafs = *_onGetLeafs();
        if(!leafs) {
            // TODO: search & extract all derive classes.
            leafs = new types();
            _setLeafs(leafs);

            _findLeafs(*this, *leafs);
        }

        return *leafs;
    }

    void me::_findLeafs(const type& cls, types& tray) const {
        for(const type* sub : cls.getSubs()) {
            if(sub->getSubs().size() == 0)
                tray.push_back(const_cast<type*>(sub));

            _findLeafs(*sub, tray);
        }
    }

    nbool me::init() {
        // pre:
        //  caution for not refering metaclass and binding inside of this:
        //  while this func is called, a structuring for metaclass doesn't finished.
        //  so if you call funcs using metaclass (in)directly, that calling makes
        //  crash or infinite loop.
        //  please you make sure not to use those APIs.
        //
        // object class should not be initialized explicitly:
        //  or it makes recursive call. because if we make a instance of ttype<object>,
        //  it triggers type::init inside of it.
        if(isInit()) return false;

        // main:
        //  setting init flag first is important:
        //      when we construct structure between super and subs,
        //      we need to assign ptr of static variable. but can't gaurantee that this ptr is
        //      belonged to static memory. so we're going to use get() static func.
        //      when static variable at get() func was not initialized before, its constructor
        //      will leds us to here, however nothing serius happen because init flag was set
        //      to true.
        _setInit(true);
        //  get supers info from super:
        //      at this point ttype<super> is instantiated, and "super" also is all of this
        //      sequences.
        type& super = const_cast<type&>(getSuper());
        super.init();
        //        constructing SuperType:
        types& mySupers = _getSupers();
        mySupers = super._getSupers();
        mySupers.push_back(&super);
        // post:
        //  notify to super:
        super._onAddSubClass(*this);
        return _logInitOk(true);
    }

    void me::_onAddSubClass(const me& subClass) {
        _getSubs().push_back(&subClass._getStatic());
    }

    nbool me::rel() {
        _setInit(false);
        return true;
    }

    nbool me::isSuper(const type& it) const {
        //  checking class hierarchy algorithm:
        //        use the "tier" of the class hierarchy info to check it.
        //        "tier" means that how this class are inherited far from the Root class, that is,
        //        object.
        //        so, if the "this" is a super of given object "it", its "tier"th super class
        //        would must be the class of "this".
        if(nul(it)) return false;
        const types& its = it.getSupers();
        ncnt myTier = getSupers().size(),
             itsTier = its.size();
        if(myTier > itsTier) return false;


        //  main:
        const type& target = itsTier == myTier ? it :
            (const type&) *its[myTier];

        return *this == target; // operator== is virtual func.
    }

    nbool me::isSub(const type& it) const {
        return it.isSuper(*this);
    }

    const void* me::getExtra() const {
        return nullptr;
    }

    nbool me::_logInitOk(nbool res) {
        if(!res)
            return NAMU_E("couldn't init meta of %s class.", getName().c_str()), res;

        return res;
    }

    const types& me::getSubs() const {
        return (const_cast<me*>(this))->_getSubs();
    }
    const types& me::getSupers() const {
        return (const_cast<me*>(this))->_getSupers();
    }

    void me::_setInit(nbool newState) {
        const nbool& res = isInit();
        nbool& isInit = const_cast<nbool&>(res);

        isInit = newState;
    }

    void me::_setLeafs(types* newLeafs) const {
        types** leafs = _onGetLeafs();
        if(*leafs == newLeafs) return;

        if(*leafs)
            delete *leafs;

        *leafs = newLeafs;
    }
}
