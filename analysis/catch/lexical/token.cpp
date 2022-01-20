#include "token.hpp"
#include "cerberus/lexical/tokens/token.hpp"

namespace cerb::test
{

    auto tokenTest(u32) -> int
    {
        lex::TextGenerator<> manager(
            "    "
            "\t\tHello, World! \nIt's a test \t\t  \nstring.");

        EXPECT_TRUE(
            manager.getLineInText() ==
            "    "
            "\t\tHello, World! ");
        EXPECT_TRUE(manager.getCharWithoutLayout() == 'H');
        EXPECT_TRUE(
            manager.getTabsAndSpaces() ==
            "    "
            "\t\t");
        EXPECT_TRUE(manager.getRawChar() == 'e');

        while (manager.getRawChar() != '\n') {
            // empty statement
        }

        EXPECT_TRUE(manager.getCharWithoutLayout() == 'I');
        lex::Token<char, int> token{ 10, manager.getLocation(), "It's", manager };

        EXPECT_TRUE(token.getType() == 10);
        EXPECT_TRUE(token.getLine() == 1);
        EXPECT_TRUE(token.getCharacter() == 1);
        EXPECT_TRUE(token.getOffset() == 21);
        EXPECT_TRUE(token.getFilename() == "");
        EXPECT_TRUE(token.getTabsAndSpaces().empty());
        EXPECT_TRUE(token.getRepr() == "It's");

        return 0;
    }
}// namespace cerb::test