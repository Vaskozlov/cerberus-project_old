#include "generator_for_text.hpp"
#include <cerberus/lexical/generator_for_text.hpp>

namespace cerb::test {
    auto txtManagerTestOnStr() -> void
    {
        lex::GeneratorForText<> manager(
            LocationInFile("None"),
            "    "
            "\t\tHello, World! \nIt's a test \t\t  \nstring.");

        EXPECT_TRUE(
            manager.getCurrentLine() ==
            "    "
            "\t\tHello, World! ");
        EXPECT_TRUE(manager.getFilename() == "None");
        EXPECT_TRUE(manager.getCharAtCurrentOffset() == ' ');
        EXPECT_TRUE(manager.skipLayoutAndGiveChar() == 'H');
        EXPECT_TRUE(
            manager.getTabsAndSpaces() ==
            "    "
            "\t\t");
        EXPECT_TRUE(manager.newRawChar() == 'e');
        EXPECT_TRUE(manager.getTabsAndSpaces().empty());
        EXPECT_TRUE(manager.getLine() == 0);
        EXPECT_TRUE(manager.getCharacterInLine() == 8);

        while (manager.newRawChar() != '\n') {
            // empty statement
        }

        EXPECT_TRUE(manager.skipLayoutAndGiveChar() == 'I');
        EXPECT_TRUE(manager.getLine() == 1);
        EXPECT_TRUE(manager.getCharacterInLine() == 1);
        EXPECT_TRUE(manager.getOffset() == 22);
        EXPECT_TRUE(manager.getCurrentLine() == "It's a test \t\t  ");
    }

    auto txtManagerTest(u32) -> int
    {
        txtManagerTestOnStr();

        std::string str = "Hello,World!It'sateststring.";
        std::string process_str{};

        lex::GeneratorForText<> manager(
            LocationInFile("None"),
            "    "
            "\t\tHello, World! \nIt's a test \t\t  \nstring.");

        for (char elem : manager) {
            process_str.push_back(elem);
        }

        EXPECT_TRUE(process_str == str);

        return 0;
    }
}// namespace cerb::test
