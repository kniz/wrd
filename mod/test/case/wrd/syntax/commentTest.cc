#include "../../../wrdSyntaxTest.hpp"

using namespace wrd;
using namespace std;

namespace {
    struct commentTest : public wrdSyntaxTest {};
}

TEST_F(commentTest, singleLineComment) {
    // control group.
    make().parse(R"SRC(
        age int // age is age
        main() int // main is also a main
            return 0
    )SRC").shouldVerified(true);
    ASSERT_EQ(getSubPack().subs().len(), 3); // 1 for age, 1 for main() 1 for @ctor
}

TEST_F(commentTest, multiLineComment) {
    // control group.
    make().parse(R"SRC(
        age int /* age is age
        main() int */
        main() flt
            return 2.5
    )SRC").shouldVerified(true);
    ASSERT_EQ(getSubPack().subs().len(), 3);
}

TEST_F(commentTest, multiLineComment2) {
    // control group.
    make().parse(R"SRC(
        age /* age is age
        main() int
 sdfas   */int
        main() void
            return
    )SRC").shouldVerified(true);
    ASSERT_EQ(getSubPack().subs().len(), 3);
}

TEST_F(commentTest, multiLineComment3) {
    // control group.
    make().parse(R"SRC(
        age int /* age is age
        main() int
 sdfas*/main() int
            return 33
    )SRC").shouldParsed(false);
/* above case is same to,
 *
 *      age int main() bool
 *          return false
 */
}
