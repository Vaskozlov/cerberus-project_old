#include "token.hpp"
#include <cerberus/lexical/token.hpp>

namespace cerb::test {
    using SimpleTokenType = unsigned;

    auto tokenTest(u32) -> int
    {
        TextGenerator<> manager(
            LocationInFile("None"),
            "    "
            "\t\tHello, World! \nIt's a test \t\t  \nstring.");

        EXPECT_TRUE(
            manager.getCurrentLine() ==
            "    "
            "\t\tHello, World! ");
        EXPECT_TRUE(manager.nextYieldedChar() == ' ');
        EXPECT_TRUE(manager.skipLayoutAndGiveNewChar() == 'H');
        EXPECT_TRUE(
            manager.getTabsAndSpaces() ==
            "    "
            "\t\t");
        EXPECT_TRUE(manager.newChar() == 'e');

        while (manager.newChar() != '\n') {
            // empty statement
        }

        EXPECT_TRUE(manager.skipLayoutAndGiveNewChar() == 'I');
        lex::Token<char, SimpleTokenType> token{
            10, manager.getLocation(), "It's", manager
        };

        EXPECT_TRUE(token.getType() == 10);
        EXPECT_TRUE(token.getLine() == 1);
        EXPECT_TRUE(token.getCharacter() == 1);
        EXPECT_TRUE(token.getOffset() == 22);
        EXPECT_TRUE(token.getFilename() == "None");
        EXPECT_TRUE(token.getTabsAndSpaces().empty());
        EXPECT_TRUE(token.getRepr() == "It's");

        return 0;
    }
}// namespace cerb::test