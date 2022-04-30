#include <cerberus/debug/debug.hpp>
#include <cerberus/lex/item/item.hpp>

namespace cerb::debug
{
    using namespace lex;
    using namespace regex;
    using namespace string;

    AnalysisGlobals<char> Parameters{};
    AnalysisGlobals<char16_t> ParametersUtf16{};

    auto dotItemErrorCaseEmptyRegex() -> void
    {
        try {
            CERBLIB_UNUSED(auto) = lex::DotItem<char>(Parameters, 0, "[]");
            CANT_BE_REACHED;
        } catch (lex::regex::RegexParser<char>::RegexParsingError const &) {
            MUST_BE_REACHED;
        }
    }

    auto dotItemErrorCaseEmptyString() -> void
    {
        try {
            CERBLIB_UNUSED(auto) = lex::DotItem<char>(Parameters, 0, "\"\"");
            CANT_BE_REACHED;
        } catch (lex::string::StringItem<char>::StringItemError const &) {
            MUST_BE_REACHED;
        }
    }

    auto dotItemErrorCaseEmptyItem() -> void
    {
        try {
            CERBLIB_UNUSED(auto) = lex::DotItem<char>(Parameters, 0, "()");
            CANT_BE_REACHED;
        } catch (AnalysisException<char> const &error) {
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
        CERBLIB_UNUSED(auto) = lex::DotItem<char>(Parameters, 0, "\'+\'");
    }

    auto testDotItemOnString() -> void
    {
        CERBLIB_UNUSED(auto) = lex::DotItem<char>(Parameters, 1, "\"Hello, World!\"");
    }

    auto testDotItemOnStringAndRegex() -> void
    {
        CERBLIB_UNUSED(auto) = lex::DotItem<char>(Parameters, 2, "\"for\"p+[a-z]*");
    }

    auto testComplexDotItem() -> void
    {
        CERBLIB_UNUSED(auto) = lex::DotItem<char>(Parameters, 3, "(\"for\"p*)+[a-z]+");
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
