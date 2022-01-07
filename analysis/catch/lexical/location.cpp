#include "location.hpp"
#include <cerberus/lexical/location.hpp>

namespace cerb::test {
    auto txtManagerTestOnStr() -> void
    {
        TextGenerator<> manager(
            LocationInFile("None"),
            "    "
            "\t\tHello, World! \nIt's a test \t\t  \nstring.");

        EXPECT_TRUE(
            manager.getCurrentLine() ==
            "    "
            "\t\tHello, World! ");
        EXPECT_TRUE(manager.getFilename() == "None");
        EXPECT_TRUE(manager.nextYieldedChar() == ' ');
        EXPECT_TRUE(manager.skipLayoutAndGiveNewChar() == 'H');
        EXPECT_TRUE(
            manager.getTabsAndSpaces() ==
            "    "
            "\t\t");
        EXPECT_TRUE(manager.newChar() == 'e');
        EXPECT_TRUE(manager.getTabsAndSpaces().empty());
        EXPECT_TRUE(manager.getLine() == 0);
        EXPECT_TRUE(manager.getCharacter() == 8);

        while (manager.newChar() != '\n') {
            // empty statement
        }

        EXPECT_TRUE(manager.skipLayoutAndGiveNewChar() == 'I');
        EXPECT_TRUE(manager.getLine() == 1);
        EXPECT_TRUE(manager.getCharacter() == 1);
        EXPECT_TRUE(manager.getOffset() == 22);
        EXPECT_TRUE(manager.getCurrentLine() == "It's a test \t\t  ");
    }

    auto txtManagerTest(u32) -> int
    {
        txtManagerTestOnStr();

        cerb::string_view str = "Hello,World!It'sateststring.";
        auto str_it = str.begin();

        TextGenerator<> manager(
            LocationInFile("None"),
            "    "
            "\t\tHello, World! \nIt's a test \t\t  \nstring.");

        for (char elem : manager) {
            EXPECT_TRUE(*str_it == elem);
            ++str_it;
        }

        return 0;
    }
}// namespace cerb::test
