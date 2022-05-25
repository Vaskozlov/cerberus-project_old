#include <cerberus/debug/debug.hpp>
#include <cerberus/lex/item/item.hpp>

namespace cerb::debug
{
    using namespace lex;
    using namespace regex;
    using namespace string;

    auto dotItemErrorCaseEmptyRegex() -> void
    {
        AnalysisGlobalsParameters<char> parameters{};

        try {
            CERBLIB_UNUSED(auto) = DotItem<char>(parameters, 0, "[]");
            CANT_BE_REACHED;
        } catch (RegexParser<char>::RegexParsingError const &error) {
            EXPECT_EQUAL(
                error.getMessage(),
                "Analysis error occurred: There are no characters in regex! File: , line: 1, char: "
                "2\n[]\n ^");
            MUST_BE_REACHED;
        }
    }

    auto dotItemErrorCaseEmptyString() -> void
    {
        AnalysisGlobalsParameters<char> parameters{};

        try {
            CERBLIB_UNUSED(auto) = DotItem<char>(parameters, 0, "\"\"");
            CANT_BE_REACHED;
        } catch (StringItem<char>::StringItemError const &error) {
            EXPECT_EQUAL(
                error.getMessage(),
                "Analysis error occurred: Empty strings are not allowed! File: , line: 1, char: 2\n"
                "\"\"\n ^");
            MUST_BE_REACHED;
        }
    }

    auto dotItemErrorCaseEmptyItem() -> void
    {
        AnalysisGlobalsParameters<char> parameters{};

        try {
            CERBLIB_UNUSED(auto) = lex::DotItem<char>(parameters, 0, "()");
            CANT_BE_REACHED;
        } catch (ItemParser<char>::DotItemParsingError const &error) {
            EXPECT_EQUAL(
                error.getMessage(),
                "Analysis error occurred: Empty items are not allowed! File: , line: 1, char: 2\n"
                "()\n ^");
            MUST_BE_REACHED;
        }
    }

    auto testDotItemCreationOnErrorCases() -> void
    {
        dotItemErrorCaseEmptyRegex();
        dotItemErrorCaseEmptyString();
        dotItemErrorCaseEmptyItem();
    }

    auto testDotItemOnNonTerminal() -> void
    {
        AnalysisGlobalsParameters<char> parameters{};
        DotItem<char> item{ parameters, 0, "\'+\'" };
        auto const &items = item.getItems();

        EXPECT_TRUE(items.empty());
    }

    auto testDotItemOnString() -> void
    {
        AnalysisGlobalsParameters<char> parameters{};
        DotItem<char> item{ parameters, 1, "\"Hello, World!\"+" };
        auto const &items = item.getItems();

        EXPECT_EQUAL(items.size(), 1);

        auto const &front_item = items.front();
        auto const *string_item = dynamic_cast<StringItem<char> *>(front_item.get());

        EXPECT_NOT_EQUAL(string_item, nullptr);

        EXPECT_TRUE(string_item->flags.isSet(ItemFlags::PLUS));
    }

    auto testDotItemOnStringAndRegex() -> void
    {
        AnalysisGlobalsParameters<char> parameters{};
        DotItem<char> item{ parameters, 2, "\"for\"p+[a-zA-Z_]*" };
        auto const &items = item.getItems();

        EXPECT_EQUAL(items.size(), 2);

        auto const &front_item = items.front();
        auto const *string_item = dynamic_cast<StringItem<char> *>(front_item.get());

        EXPECT_NOT_EQUAL(string_item, nullptr);

        EXPECT_TRUE(string_item->getString() == "for");
        EXPECT_TRUE(string_item->flags.isSet(ItemFlags::PLUS));
        EXPECT_TRUE(string_item->flags.isSet(ItemFlags::PREFIX));

        auto const &back_item = items.back();
        auto const *regex_item = dynamic_cast<RegexItem<char> *>(back_item.get());

        EXPECT_NOT_EQUAL(regex_item, nullptr);

        EXPECT_TRUE(regex_item->flags.isSet(ItemFlags::STAR));
    }

    auto testComplexDotItem() -> void
    {
        AnalysisGlobalsParameters<char> parameters{};
        DotItem<char> item{ parameters, 3, "(\"for\"p*)+[a-zA-Z_]+" };

        auto const &items = item.getItems();

        EXPECT_EQUAL(items.size(), 2);

        auto const &front_item = items.front();
        auto const *parsing_item = dynamic_cast<ItemParser<char> *>(front_item.get());

        EXPECT_NOT_EQUAL(parsing_item, nullptr);

        EXPECT_TRUE(parsing_item->flags.isSet(ItemFlags::PLUS));

        auto const &back_item = items.back();
        auto const *regex_item = dynamic_cast<RegexItem<char> *>(back_item.get());

        EXPECT_NOT_EQUAL(regex_item, nullptr);
        EXPECT_TRUE(regex_item->flags.isSet(ItemFlags::PLUS));
    }

    auto testDotItem() -> int
    {
        testDotItemOnNonTerminal();
        testDotItemOnString();
        testDotItemOnStringAndRegex();
        testComplexDotItem();
        testDotItemCreationOnErrorCases();
        return 0;
    }
}// namespace cerb::debug
