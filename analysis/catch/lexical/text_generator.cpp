#include "generator_for_text.hpp"
#include <cerberus/lexical/text_generator.hpp>

namespace cerb::test
{
    auto txtManagerTestOnStr() -> void
    {
        lex::TextGenerator<> manager(
            "    "
            "\t\tHello, World! \nIt's a test \t\t  \nstring.",
            "None");

        EXPECT_TRUE(
            manager.getLineInText() ==
            "    "
            "\t\tHello, World! ");
        EXPECT_TRUE(manager.getFilename() == "None");
        EXPECT_TRUE(manager.getCurrentChar() == '\0');
        EXPECT_TRUE(manager.getCharWithoutLayout() == 'H');
        EXPECT_TRUE(
            manager.getTabsAndSpaces() ==
            "    "
            "\t\t");
        EXPECT_TRUE(manager.getRawChar() == 'e');
        EXPECT_TRUE(manager.getTabsAndSpaces().empty());
        EXPECT_TRUE(manager.getLineNumber() == 1);
        EXPECT_TRUE(manager.getCharNumber() == 8);

        while (manager.getRawChar() != '\n') {
            // empty statement
        }

        EXPECT_TRUE(manager.getCharWithoutLayout() == 'I');
        EXPECT_TRUE(manager.getLineNumber() == 2);
        EXPECT_TRUE(manager.getCharNumber() == 1);
        EXPECT_TRUE(manager.getOffset() == 21);
        EXPECT_TRUE(manager.getLineInText() == "It's a test \t\t  ");
    }

    auto txtManagerTest(u32) -> int
    {
        txtManagerTestOnStr();

        std::string str = "Hello,World!It'sateststring.";
        std::string process_str{};

        lex::TextGenerator<> manager(
            "    "
            "\t\tHello, World! \nIt's a test \t\t  \nstring.");

        while (manager.getCharWithoutLayout() != '\0') {
            process_str.push_back(manager.getCurrentChar());
        }

        EXPECT_TRUE(process_str == str);


        return 0;
    }
}// namespace cerb::test
