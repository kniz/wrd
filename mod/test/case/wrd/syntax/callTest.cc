#include "../../../syntaxTest.hpp"

using namespace wrd;
using namespace std;

namespace {
    struct callTest : public syntaxTest {};
}

TEST_F(callTest, callFunc) {
    make().parse(R"SRC(
        pack demo
        main() void
            foo()
    )SRC").shouldParsed(true);
}

TEST_F(callTest, callFunc1) {
    make().parse(R"SRC(
        main() void
            a.foo()
    )SRC").shouldParsed(true);
}

TEST_F(callTest, callFunc2) {
    make().parse(R"SRC(
        main() void
            --a++.foo()
    )SRC").shouldParsed(true);
}

TEST_F(callTest, callFunc3) {
    make().parse(R"SRC(
        main() void
            (--a++.foo()).doo().goo()
    )SRC").shouldParsed(true);
}

TEST_F(callTest, callFuncShouldNotExistOnPack) {
    make().parse(R"SRC(
        foo()
    )SRC").shouldParsed(false);
}

TEST_F(callTest, callFuncWithExprInside) {
    make().parse(R"SRC(
        main() void
            a.foo(22, 34)
    )SRC").shouldParsed(true);
}

TEST_F(callTest, callFuncWithExprInside1) {
    make().parse(R"SRC(
        main() void
            if "test"
                a.foo(22, 34, boo(22))
    )SRC").shouldParsed(true);
}

TEST_F(callTest, callFuncWithExprInside2) {
    make().parse(R"SRC(
        main() void
            if "test"
                a.foo(22, 34, boo(a int) void
                    b.boo(a)
                )
    )SRC").shouldParsed(true);
}

TEST_F(callTest, callFuncWithExprInside3) {
    make().parse(R"SRC(
        main() void
            if "test"
                a.foo(22, 34, (boo(a int) void
                    b.boo(a)
                ))
    )SRC").shouldParsed(true);
}
