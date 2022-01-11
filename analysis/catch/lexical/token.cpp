#include "token.hpp"
#include <cerberus/lexical/token.hpp>

namespace cerb::test {

    auto tokenTest(u32) -> int
    {
        lex::GeneratorForText<> manager(
            LocationInFile{"None"},
            "    "
            "\t\tHello, World! \nIt's a test \t\t  \nstring.");

        EXPECT_TRUE(
            manager.getCurrentLine() ==
            "    "
            "\t\tHello, World! ");
        EXPECT_TRUE(manager.getCharAtCurrentOffset() == ' ');
        EXPECT_TRUE(manager.skipLayoutAndGiveChar() == 'H');
        EXPECT_TRUE(
            manager.getTabsAndSpaces() ==
            "    "
            "\t\t");
        EXPECT_TRUE(manager.newRawChar() == 'e');

        while (manager.newRawChar() != '\n') {
            // empty statement
        }

        EXPECT_TRUE(manager.skipLayoutAndGiveChar() == 'I');
        lex::Token<char> token{ 10, manager.getLocation(), "It's", manager };

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