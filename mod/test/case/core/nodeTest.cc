#include <utility>

#include "../../namuTest.hpp"

using namespace nm;
using namespace std;

struct nodeTest: public namuTest {};

namespace {
    class myFunc: public func {
        NM(CLASS(myFunc, func))

    public:
        myFunc(): super(params(), new nVoid()) {}

        void setUp() { _executed = false; }

        str run(const args& a) override {
            NM_I("hello world!");
            _executed = true;
            return str();
        }

        nbool isRun() const { return _executed; }

        str getRet() const override {
            static str inner(new nVoid());
            return inner;
        }

        const params& getParams() const override {
            static params inner;
            return inner;
        }

    private:
        nbool _executed;
    };

    struct myObj: public obj {
        NM(CLASS(myObj, obj))

    public:
        myObj(int newVal = 0): val(newVal) {}

        int val;

        nbool _onSame(const typeProvidable& rhs) const override {
            const myObj& cast = (const myObj&) rhs;
            return val == cast.val;
        }
    };

    class food: public obj {
        NM(CLASS(food, obj))

    public:
        food(string newName, int newCalorie): name(std::move(newName)), calorie(newCalorie) {}

        string name;
        int calorie;
    };

    class chef: public obj {
        class myType: public ntype {
            NM_ME(myType, ntype)

        protected:
            const ases& _getImpliAses() const override {
                static ases* inner = nullptr;
                if(inner) return *inner;

                inner = new ases();

                struct tofood: public tas<food> {
                    str as(const node& it, const type& to) const override {
                        const chef& chef1 = it.cast<chef>();
                        if(nul(chef1)) return str();

                        return str(new food(chef1.foodName, chef1.foodCalorie));
                    }
                };

                inner->add(new tofood());

                return *inner;
            }
        };
        NM(CLASS(chef, obj, myType))

    public:
        string foodName;
        int foodCalorie;
    };
} // namespace

TEST_F(nodeTest, testManuallyMakeNodeStructure) {
    // prepare:
    tstr<scope> frameEmulator;
    myObj obj;
    myFunc func;
    NM_DI("func.tag.chkId=%d", func.getBindTag().getId().chkN);

    obj.subs().add("myFunc", func);
    myFunc funcOffunc;
    func.subs().add("funcOfFunc", funcOffunc);

    NM_DI("func.tag.chkId=%d", func.getBindTag().getId().chkN);
    NM_DI("funcOffunc.tag.chkId=%d", funcOffunc.getBindTag().getId().chkN);

    // when:
    frameEmulator.bind(obj.subs());
    //  push another:
    scope* chnOffunc = scope::wrap<scope>(func.subs());
    chnOffunc->link(*frameEmulator);
    frameEmulator.bind(*chnOffunc);

    NM_DI("func.tag.chkId=%d", func.getBindTag().getId().chkN);
    NM_DI("funcOffunc.tag.chkId=%d", funcOffunc.getBindTag().getId().chkN);

    // then:
    ASSERT_EQ(chnOffunc->len(), 2);
    ASSERT_EQ(chnOffunc->len(), 2);
    ASSERT_EQ(chnOffunc->len(), 2);

    int n = 0;
    for(const auto& elem: *chnOffunc)
        NM_I("[%d]=%s", n++, elem);

    NM_DI("func.tag.chkId=%d", func.getBindTag().getId().chkN);
    NM_DI("funcOffunc.tag.chkId=%d", funcOffunc.getBindTag().getId().chkN);

    ASSERT_EQ(n, 2);
    NM_DI("func.tag.chkId=%d", func.getBindTag().getId().chkN);
    NM_DI("funcOffunc.tag.chkId=%d", funcOffunc.getBindTag().getId().chkN);
}

TEST_F(nodeTest, testManualNativefuncCall) {
    // prepare:
    myFunc func;

    myObj obj;
    obj.subs().add("myFunc", func);

    args a;
    auto subs = ((const myObj&) obj).subAll("myFunc", a);
    ASSERT_EQ(subs.len(), 1);
    ASSERT_TRUE(subs[0].canRun(a));

    // when:
    func.setUp();
    subs[0].run(a);
    ASSERT_TRUE(func.isRun());
}

TEST_F(nodeTest, testchefImplicitCastTofood) {
    // prepare:
    const string expectName = "HealthPotion";
    const int expectCalorie = 350;
    chef chef;
    chef.foodName = expectName;
    chef.foodCalorie = expectCalorie;

    const ntype& chefType = chef.getType();
    ASSERT_TRUE(chefType.isImpli<food>());
    tstr<food> cast = chef.asImpli<food>();
    ASSERT_TRUE(cast);

    EXPECT_EQ(cast->name, expectName);
    EXPECT_EQ(cast->calorie, expectCalorie);
}

TEST_F(nodeTest, ShouldNotSameNameVariableIntoSubs) {
    chef c;
    ASSERT_EQ(c.subs().len(), 0);
    c.subs().add("age", new nInt(22));

    ASSERT_EQ(c.subs().len(), 1);
    ASSERT_EQ(c.sub<nInt>("age").get(), 22);
    errReport rpt;
    verifier v;
    v.setReport(rpt).setTask(c).work();
    ASSERT_FALSE(rpt);

    c.subs().add("age1", new nInt(22));
    ASSERT_EQ(c.subs().len(), 2);
    auto matches = c.subAll<nInt>("age1");
    ASSERT_TRUE(matches.isMatched());
    ASSERT_EQ(matches.get().get(), 22);

    v.setTask(c).work();
    ASSERT_FALSE(rpt);

    c.subs().add("age", new nInt(23));
    ASSERT_EQ(c.subs().len(), 3);
    ASSERT_EQ(c.subAll<nInt>("age").len(), 2);
    v.setTask(c).work();
    ASSERT_TRUE(rpt);
}
