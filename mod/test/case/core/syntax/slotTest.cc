#include <utility>

#include "../../../namuSyntaxTest.hpp"

using namespace nm;
using namespace std;

namespace {
    struct slotTest: public namuSyntaxTest {};

    class myfunc: public func {
        NM(ME(myfunc, func), CLONE(myfunc))

        class myBlock: public blockExpr {
            NM(CLASS(myBlock, blockExpr))

        public:
            str run(const args& a) override {
                NM_I("hello world!");
                _executed = true;

                if(_lambda) _res = _lambda(a, (frames&) nm::thread::get().getFrames());
                return str();
            }

            void setLambda(function<nbool(const ucontainable&, const frames&)> lambda) {
                _lambda = std::move(lambda);
            }

            function<nbool(const ucontainable&, const frames&)> _lambda;
            nbool _res;
            nbool _executed;
        };

    public:
        myfunc():
            super(*new modifier(), params(), new nVoid(), *new myBlock()),
            _type("myfunc", ttype<me>::get(), params()) {
            NM_I("myfunc(%s) new", this);
        }

        ~myfunc() override { NM_I("myfunc(%s) delete", this); }

        const ntype& getType() const override { return _type; }

        nbool isRun() const { return getBlock().cast<myBlock>()._executed; }

        void setLambda(function<nbool(const ucontainable&, const frames&)> lambda) {
            getBlock().cast<myBlock>()._lambda = std::move(lambda);
        }

        nbool isSuccess() const { return getBlock().cast<myBlock>()._res; }

        str getRet() const override {
            static str inner(new nVoid());
            return inner;
        }

        mgdType& getType() { return _type; }

    private:
        mgdType _type;
    };

    class nativeFunc: public baseFunc {
        NM(ME(nativeFunc, baseFunc), CLONE(nativeFunc))

    public:
        nativeFunc(): super(*new modifier) {}

    public:
        nbool isRun() const { return _executed; }

        void setLambda(function<nbool(const ucontainable&, const frames&)> lambda) {
            _lambda = std::move(lambda);
        }

        nbool isSuccess() const { return _res; }

        str getRet() const override {
            static str inner(new nVoid());
            return inner;
        }

        using super::getParams;

        params& getParams() override { return _params; }

        str run(const args& a) override {
            _executed = true;
            _res = _lambda(a, (frames&) nm::thread::get().getFrames());
            return str();
        }

    private:
        function<nbool(const ucontainable&, const frames&)> _lambda;
        params _params;
        nbool _res;
        nbool _executed;
    };
} // namespace

TEST_F(slotTest, parsePackTest) {
    make(manifest("demo"))
        .parse(R"SRC(
pack demo
    )SRC")
        .shouldParsed(true);
    ASSERT_FALSE(nul(getSubPack()));
    ASSERT_FALSE(nul(getSlot().subs()));
    auto& shares = (scope::super&) getSlot().subs().getNext().getContainer();
    ASSERT_FALSE(nul(shares));
    ASSERT_EQ(shares.len(), 2);
    ASSERT_EQ(getSlot().getManifest().name, "demo");
}

TEST_F(slotTest, slotIsInFrameWhenCallMgdFunc) {
    // check whether pack's subnodes registered into frame when it calls:
    slot testSlot(manifest("demo"));
    myfunc f1;

    params& ps = f1.getParams();
    ps.add(new param("age", new nInt()));
    ps.add(new param("grade", new nFlt()));
    f1.setLambda([](const auto& contain, const auto& sf) {
        const frame& fr = sf[sf.len() - 1];
        if(nul(fr)) return false;

        // checks slot is in frame:
        const myfunc& cast = fr.sub<myfunc>("foo", narr(nInt(), nFlt()));
        if(nul(cast)) return false;

        const params& ps = cast.getParams();
        if(nul(ps)) return false;
        if(ps.len() != 2) return false;
        if(ps[0].getOrigin().getType() != ttype<nInt>()) return false;
        if(ps[1].getName() != "grade") return false;

        // checks args of funcs is in frame:
        const nInt& age = fr.sub<nInt>("age");
        if(nul(age)) return false;
        if(age.cast<int>() != 1) return false;

        const nFlt& grade = fr.sub("grade").cast<nFlt>();
        if(nul(grade)) return false;
        if(grade.get() < 3.4f || grade.get() > 3.6f) return false;

        return true;
    });

    testSlot.subs().add("foo", f1);
    testSlot.run("foo", narr(nInt(1), nFlt(3.5f)));
    ASSERT_TRUE(f1.isRun());
    ASSERT_TRUE(f1.isSuccess());
}

/* Concept changed: now, native call also make a frame instance.
 * TEST_F(slotTest, slotIsNotInFrameWhenCallNativeFunc) {
    // check whether slot's subnodes not registered into frame when it calls:
    slot testPack(manifest("demo"), packLoadings());
    nativeFunc f1;
    params& ps = f1.getParams();
    ps.add(new param("age", ttype<nInt>::get()));
    ps.add(new param("grade", ttype<nFlt>::get()));
    f1.setLambda([](const auto& contain, const auto& sf) {
        const frame& fr = sf[sf.len() - 1];
        if(!nul(fr)) return NM_E("fr == null"), false;

        return true;
    });
    testPack.subs().add("foo", f1);

    testPack.run("foo", narr(nInt(1), nFlt(3.5f)));
    ASSERT_TRUE(f1.isRun());
    ASSERT_TRUE(f1.isSuccess());
}*/
