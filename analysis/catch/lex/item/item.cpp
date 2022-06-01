#include <cerberus/debug/debug.hpp>
#include <cerberus/lex/item/item.hpp>

namespace cerb::debug
{
    using namespace lex;
    using namespace regex;
    using namespace string;

    auto dotItemErrorCaseEmptyRegex() -> void
    {
        AnalysisGlobals<char> parameters{};

        ERROR_EXPECTED(
            CERBLIB_UNUSED(auto) = DotItem<char>(parameters, 0, "[]"),
            RegexParser<char>::RegexParsingError,
            "Analysis error occurred: There are not any characters in regex! File: , line: 1, "
            "char: 2\n[]\n ^")
    }

    auto dotItemErrorCaseEmptyString() -> void
    {
        AnalysisGlobals<char> parameters{};

        ERROR_EXPECTED(
            CERBLIB_UNUSED(auto) = DotItem<char>(parameters, 0, "\"\""),
            StringItem<char>::StringItemError,
            "Analysis error occurred: Empty strings are not allowed! File: , line: 1, char: 2\n"
            "\"\"\n ^")
    }

    auto dotItemErrorCaseEmptyItem() -> void
    {
        AnalysisGlobals<char> parameters{};

        ERROR_EXPECTED(
            CERBLIB_UNUSED(auto) = lex::DotItem<char>(parameters, 0, "()"),
            ItemParser<char>::DotItemParsingError,
            "Analysis error occurred: Empty items are not allowed! File: , line: 1, char: 2\n"
            "()\n ^")
    }

    auto testDotItemCreationOnErrorCases() -> void
    {
        dotItemErrorCaseEmptyRegex();
        dotItemErrorCaseEmptyString();
        dotItemErrorCaseEmptyItem();
    }

    auto testDotItemOnNonTerminal() -> void
    {
        AnalysisGlobals<char> parameters{};
        DotItem<char> item{ parameters, 0, "\'+\'" };
        auto const &items = item.getItems();

        ASSERT_TRUE(items.empty());
    }

    auto testDotItemOnString() -> void
    {
        AnalysisGlobals<char> parameters{};
        DotItem<char> item{ parameters, 1, "\"Hello, World!\"+" };
        auto const &items = item.getItems();

        ASSERT_EQUAL(items.size(), 1);

        auto const &front_item = items.front();
        auto const *string_item = dynamic_cast<StringItem<char> *>(front_item.get());

        ASSERT_NOT_EQUAL(string_item, nullptr);

        ASSERT_TRUE(string_item->flags.isSet(ItemFlags::PLUS));
    }

    auto testDotItemOnStringAndRegex() -> void
    {
        AnalysisGlobals<char> parameters{};
        DotItem<char> item{ parameters, 2, "\"for\"^p+[a-zA-Z_]*" };
        auto const &items = item.getItems();

        ASSERT_EQUAL(items.size(), 2);

        auto const &front_item = items.front();
        auto const *string_item = dynamic_cast<StringItem<char> *>(front_item.get());

        ASSERT_NOT_EQUAL(string_item, nullptr);

        ASSERT_TRUE(string_item->getString() == "rof");
        ASSERT_TRUE(string_item->flags.isSet(ItemFlags::PLUS));
        ASSERT_TRUE(string_item->flags.isSet(ItemFlags::PREFIX));
        ASSERT_TRUE(string_item->flags.isSet(ItemFlags::REVERSE));

        auto const &back_item = items.back();
        auto const *regex_item = dynamic_cast<RegexItem<char> *>(back_item.get());

        ASSERT_NOT_EQUAL(regex_item, nullptr);

        ASSERT_TRUE(regex_item->flags.isSet(ItemFlags::STAR));
    }

    auto testComplexDotItem() -> void
    {
        AnalysisGlobals<char> parameters{};
        DotItem<char> item{ parameters, 3, "(\"for\"p*)+[a-zA-Z_]+" };

        auto const &items = item.getItems();

        ASSERT_EQUAL(items.size(), 2);

        auto const &front_item = items.front();
        auto const *parsing_item = dynamic_cast<ItemParser<char> *>(front_item.get());

        ASSERT_NOT_EQUAL(parsing_item, nullptr);

        ASSERT_TRUE(parsing_item->flags.isSet(ItemFlags::PLUS));

        auto const &back_item = items.back();
        auto const *regex_item = dynamic_cast<RegexItem<char> *>(back_item.get());

        ASSERT_NOT_EQUAL(regex_item, nullptr);

        ASSERT_TRUE(regex_item->flags.isSet(ItemFlags::PLUS));
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
