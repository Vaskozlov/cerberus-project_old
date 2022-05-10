#include <cerberus/debug/debug.hpp>
#include <cerberus/lex/item/item.hpp>

namespace cerb::debug
{
    using namespace lex;
    using namespace regex;
    using namespace string;

    // NOLINTBEGIN
    AnalysisGlobalsParameters<char> Parameters{};
    AnalysisGlobalsParameters<char16_t> ParametersUtf16{};
    // NOLINTEND

    auto dotItemErrorCaseEmptyRegex() -> void
    {
        try {
            CERBLIB_UNUSED(auto) = DotItem<char>(Parameters, 0, "[]");
            CANT_BE_REACHED;
        } catch (RegexParser<char>::RegexParsingError const &) {
            MUST_BE_REACHED;
        }
    }

    auto dotItemErrorCaseEmptyString() -> void
    {
        try {
            CERBLIB_UNUSED(auto) = DotItem<char>(Parameters, 0, "\"\"");
            CANT_BE_REACHED;
        } catch (StringItem<char>::StringItemError const &) {
            MUST_BE_REACHED;
        }
    }

    auto dotItemErrorCaseEmptyItem() -> void
    {
        try {
            CERBLIB_UNUSED(auto) = lex::DotItem<char>(Parameters, 0, "()");
            CANT_BE_REACHED;
        } catch (BasicLexicalAnalysisException const &error) {
            ::fmt::print("{}\n", error.what());
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
        DotItem<char> item{ Parameters, 0, "\'+\'" };
        auto const &items = item.getItems();

        EXPECT_TRUE(items.empty());
    }

    auto testDotItemOnString() -> void
    {
        DotItem<char> item{ Parameters, 1, "\"Hello, World!\"+" };
        auto const &items = item.getItems();

        EXPECT_TRUE(items.size() == 1);

        auto const &front_item = items.front();
        auto *string_item = dynamic_cast<StringItem<char> *>(front_item.get());

        EXPECT_TRUE(string_item != nullptr);
        EXPECT_TRUE(string_item->flags.isSet(ItemFlags::PLUS));
    }

    auto testDotItemOnStringAndRegex() -> void
    {
        DotItem<char> item{ Parameters, 2, "\"for\"p+[a-zA-Z_]*" };
        auto const &items = item.getItems();

        EXPECT_TRUE(items.size() == 2);

        auto const &front_item = items.front();
        auto *string_item = dynamic_cast<StringItem<char> *>(front_item.get());

        EXPECT_TRUE(string_item != nullptr);
        EXPECT_TRUE(string_item->getString() == "for");
        EXPECT_TRUE(string_item->flags.isSet(ItemFlags::PLUS));
        EXPECT_TRUE(string_item->flags.isSet(ItemFlags::PREFIX));

        auto const &back_item = items.back();
        auto *regex_item = dynamic_cast<RegexItem<char> *>(back_item.get());

        EXPECT_TRUE(regex_item != nullptr);
        EXPECT_TRUE(regex_item->flags.isSet(ItemFlags::STAR));
    }

    auto testComplexDotItem() -> void
    {
        DotItem<char> item{ Parameters, 3, "(\"for\"p*)+[a-zA-Z_]+" };

        auto const &items = item.getItems();

        EXPECT_TRUE(items.size() == 2);

        auto const &front_item = items.front();
        auto *parsing_item = dynamic_cast<ItemParser<char> *>(front_item.get());

        EXPECT_TRUE(parsing_item != nullptr);
        EXPECT_TRUE(parsing_item->flags.isSet(ItemFlags::PLUS));

        auto const &back_item = items.back();
        auto *regex_item = dynamic_cast<RegexItem<char> *>(back_item.get());

        EXPECT_TRUE(regex_item != nullptr);
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
