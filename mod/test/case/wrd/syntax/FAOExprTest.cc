#include "../../../wrdSyntaxTest.hpp"

using namespace wrd;
using namespace std;

namespace {
    struct FAOExprTest : public wrdSyntaxTest {};
}

TEST_F(FAOExprTest, simpleAdd) {
    make().parse(R"SRC(
        a := 5
        b := 2
        main() int
            return a + b
    )SRC").shouldVerified(true);

    wInt& a = getSubPack().sub<wInt>("a");
    ASSERT_FALSE(nul(a));
    ASSERT_EQ(a.get(), 5);
    wInt b = getSubPack().sub<wInt>("b");
    ASSERT_EQ(b.get(), 2);

    tstr<wInt> res(getSubPack().run("main"));
    ASSERT_TRUE(res);
    ASSERT_EQ(res->get(), 7);
}

TEST_F(FAOExprTest, addWithDefAssign) {
    make().parse(R"SRC(
        a := 5
        b := a + 2
        main() int
            return a + b
    )SRC").shouldVerified(true);

    wInt& a = getSubPack().sub<wInt>("a");
    ASSERT_FALSE(nul(a));
    ASSERT_EQ(a.get(), 5);
    wInt b = getSubPack().sub<wInt>("b");
    ASSERT_EQ(b.get(), 7);

    tstr<wInt> res(getSubPack().run("main"));
    ASSERT_TRUE(res);
    ASSERT_EQ(res->get(), 12);
}

TEST_F(FAOExprTest, addWithDefAssignReversedNegative) {
    make().parse(R"SRC(
        b := a + 2
        a := 5
        main() int
            return a + b
    )SRC").shouldVerified(false);

    wInt& a = getSubPack().sub<wInt>("a");
    ASSERT_FALSE(nul(a));
    ASSERT_EQ(a.get(), 5);
    wInt& b = getSubPack().sub<wInt>("b");
    ASSERT_TRUE(nul(b));
}

TEST_F(FAOExprTest, addIntAndStrNegative) {
    make().parse(R"SRC(
        a := "hello" + 12
        main() int
            return a
    )SRC").shouldParsed(true);
    shouldVerified(false);
}

TEST_F(FAOExprTest, addIntAndStr) {
    make().parse(R"SRC(
        a := "hello" + 12
        main() int
            return 0
    )SRC").shouldParsed(true);
    shouldVerified(true);

    wStr& a = getSubPack().sub<wStr>("a");
    ASSERT_FALSE(nul(a));
    ASSERT_EQ(a.get(), std::string("hello12"));
}

TEST_F(FAOExprTest, simpleSub) {
    make().parse(R"SRC(
        a := 5
        b := 2
        main() int
            return a - b
    )SRC").shouldVerified(true);

    wInt& a = getSubPack().sub<wInt>("a");
    ASSERT_FALSE(nul(a));
    ASSERT_EQ(a.get(), 5);
    wInt b = getSubPack().sub<wInt>("b");
    ASSERT_EQ(b.get(), 2);

    tstr<wInt> res(getSubPack().run("main"));
    ASSERT_TRUE(res);
    ASSERT_EQ(res->get(), 3);
}

TEST_F(FAOExprTest, modWithDefAssign) {
    make().parse(R"SRC(
        a := 10
        b := a / 2
        main() int
            return a * b % 7
    )SRC").shouldVerified(true);

    wInt& a = getSubPack().sub<wInt>("a");
    ASSERT_FALSE(nul(a));
    ASSERT_EQ(a.get(), 10);
    wInt b = getSubPack().sub<wInt>("b");
    ASSERT_EQ(b.get(), 5);

    tstr<wInt> res(getSubPack().run("main"));
    ASSERT_TRUE(res);
    ASSERT_EQ(res->get(), 1);
}

TEST_F(FAOExprTest, testStringAddSequence) {
    make().parse(R"SRC(
    Helloworld(age int) int
        return age

    main() str
        return Helloworld(4) + "low\n"
    )SRC").shouldVerified(true);

    wrd::str res = run();
    ASSERT_EQ(res->cast<std::string>(), "4low\n");
}

TEST_F(FAOExprTest, testStringAddBoolean) {
    make().parse(R"SRC(
    Helloworld(age int) int
        return age

    main() str
        return (Helloworld(false) + "low\n" )
    )SRC").shouldParsed(true);
    shouldVerified(true);

    wrd::str res = run();
    ASSERT_EQ(res->cast<std::string>(), "0low\n");
}
