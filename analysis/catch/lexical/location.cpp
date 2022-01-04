#include "location.hpp"
#include <cerberus/lexical/location.hpp>

namespace cerb::test {
    auto txtManagerTestOnStr() -> void
    {
        TxtManager<> manager(
            LocationInFile("None"),
            "    "
            "\t\tHello, World! \nIt's a test \t\t  \nstring.");

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

        while (manager.newChar() != '\n') {}

        EXPECT_TRUE(manager.skipLayoutAndGiveNewChar() == 'I');
        EXPECT_TRUE(manager.getLine() == 1);
        EXPECT_TRUE(manager.getCharacter() == 1);
        EXPECT_TRUE(manager.getOffset() == 22);
    }

    auto txtManagerTest(u32) -> int
    {
        txtManagerTestOnStr();

        return 0;
    }
}// namespace cerb::test
