#include "../../wrdTest.hpp"

using namespace wrd;
using namespace std;

struct packLoaderTest : public wrdTest {};

TEST_F(packLoaderTest, testDefaultLoaderInit) {
    const packs& systemPacks = wrd::thread::get().getSystemPacks();
    ASSERT_FALSE(nul(systemPacks));

    pack& pak = systemPacks.get([](const std::string& name, const pack& e) { return name == "samplePack"; });
    ASSERT_FALSE(nul(pak));

    ASSERT_EQ(pak.subs().len(), 1);
    node& origin = pak["helloWorld"];
    ASSERT_FALSE(nul(origin));
    {
        func& sayFunc = origin["say"].cast<func>();
        ASSERT_FALSE(nul(sayFunc));

        ASSERT_EQ(sayFunc.getEvalType(), wVoid().getType());
        ASSERT_EQ(sayFunc.getParams().len(), 0); // 'me' of func won't be passed as an argument.

        narr args {origin};
        str res = sayFunc.run(args);
        ASSERT_FALSE(res); // don't run func itself.
        res = origin.run("say", args); // don't need to pass 'me' argument
        ASSERT_FALSE(res);
        res = origin.run("say", narr());
        ASSERT_TRUE(res);
        ASSERT_EQ(res->cast<wVoid>(), wVoid());
    }

    {
        func& add = origin["add"].cast<func>();
        ASSERT_FALSE(nul(add));

        const params& argTypes = add.getParams();
        ASSERT_EQ(argTypes.len(), 2);
        ASSERT_EQ(argTypes[0].getOrigin().getType(), ttype<wInt>());
        ASSERT_EQ(argTypes[1].getOrigin().getType(), ttype<wInt>::get());

        wInt arg1(5);
        str retVal = add.run(narr {arg1} ); // should nothing happen
        ASSERT_FALSE(retVal);

        narr args;
        args.add(new wInt(5));
        args.add(new wInt(3));
        retVal = add.run(args); // don't run func itself as I said before
        ASSERT_FALSE(retVal);
        retVal = origin.run("add", args); // correct!
        ASSERT_TRUE(retVal);
        ASSERT_EQ(retVal->cast<wInt>().get(), 8);
    }
}
