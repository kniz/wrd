#include "../../../namuSyntaxTest.hpp"

using namespace namu;
using namespace std;

namespace {
    struct opTest : public namuSyntaxTest {};
}

TEST_F(opTest, simpleBinaryOp1) {
    make("demo").parse(R"SRC(
        pack demo
        main() void
            2 + 3*27 + 44 - 27/34*43 - 1
    )SRC").shouldParsed(true);
}

TEST_F(opTest, BinaryOpWithParan) {
    make("demo").parse(R"SRC(
        pack demo
        main() void
         2 + (if 3
          3*27
         )+ 44 - 27/34*43 - 1
    )SRC").shouldParsed(true);
}

TEST_F(opTest, BinaryOpWithParan2) {
    make("demo").parse(R"SRC(
        pack demo
        main() void
         2 + (if 3
          3*27)+ 44 - 27/34*43 - 1
    )SRC").shouldParsed(false);
}

TEST_F(opTest, BinaryOpWithParan3) {
    make("demo").parse(R"SRC(
         pack demo
         main() void
          2 + if 3
           3*27
          + 44 - 27/34*43 - 1
    )SRC").shouldParsed(false);
}

TEST_F(opTest, defexprAsTerm) {
    make("demo").parse(R"SRC(
        pack demo
        main() void
           (age int) + (if 3
            3*27
           ) + (grade flt) - 27/34*43 - 1
    )SRC").shouldParsed(true);
}

TEST_F(opTest, exprAddFuncCall) {
    make("demo").parse(R"SRC(
        pack demo
        main() void
            2 + 3*27 + 44 - foo(2) * 27/34*43
    )SRC").shouldParsed(true);
}

TEST_F(opTest, exprAddFuncCall2) {
    make("demo").parse(R"SRC(
        pack demo
        main() void
            2 + 3*27 + 44 - foo('a') * 27/34*43 - a.foo(b.boo(c.goo()))
    )SRC").shouldParsed(true);
}

TEST_F(opTest, exprAddFuncCall3) {
    make("demo").parse(R"SRC(
        pack demo
        main() void
            2 + 3*27 + 44 - foo("a12") * 27/34*43 - a.foo(b.boo(c.goo()))
    )SRC").shouldParsed(true);
    shouldVerified(false);
}

TEST_F(opTest, IgnoreWhitespaceAfterBinaryOpAndComma) {
    make().parse(R"SRC(
    main() void
        2 +
      3)SRC").shouldParsed(true);
}

TEST_F(opTest, IgnoreWhitespaceAfterBinaryOpAndComma2) {
    make().parse(R"SRC(
        main() void
            activateFrame(ContextManager,
            view.update(context),
            22,
            34) + 2 +
      3)SRC").shouldParsed(true);
}

TEST_F(opTest, IgnoreWhitespaceAfterBinaryOpAndComma3) {
    make().parse(R"SRC(
        main() void
            activateFrame(ContextManager,
                view.update(context),
        22,
                            34) + 2 +
      3
            butNextLine.ShouldStartHere()
    )SRC").shouldParsed(true);
}

TEST_F(opTest, IgnoreWhitespaceAfterBinaryOpAndComma4) {
    make().parse(R"SRC(
        main() void
            activateFrame(
             ContextManager,
               view.update(
                context),
        22,
                            34) +
                             2 +
    3)SRC").shouldParsed(true);
}
