#include <cerberus/debug/debug.hpp>
#include <cerberus/lex/item/item.hpp>

namespace cerb::debug
{
    using namespace lex;
    using namespace regex;
    using namespace string;

    AnalysisGlobals<char> Parameters{};

    auto dotItemErrorCaseEmptyRegex() -> void
    {
        try {
            CERBLIB_UNUSED(auto) = lex::DotItem<char>(Parameters, 0, "[]");
            CANT_BE_REACHED;
        } catch (RegexParsingError const &) {
            MUST_BE_REACHED;
        }
    }

    auto dotItemErrorCaseEmptyString() -> void
    {
        try {
            CERBLIB_UNUSED(auto) = lex::DotItem<char>(Parameters, 0, "\"\"");
            CANT_BE_REACHED;
        } catch (StringItemError const &) {
            MUST_BE_REACHED;
        }
    }

    auto dotItemErrorCaseEmptyItem() -> void
    {
        try {
            CERBLIB_UNUSED(auto) = lex::DotItem<char>(Parameters, 0, "()");
            CANT_BE_REACHED;
        } catch (DotItemParsingError const &) {
            MUST_BE_REACHED;
        }
    }

    auto testDotItemCreationOnErrorCases() -> void
    {
        dotItemErrorCaseEmptyRegex();
        dotItemErrorCaseEmptyString();
        dotItemErrorCaseEmptyItem();
    }

    auto testDotItem() -> int
    {
        CERBLIB_UNUSED(auto) = lex::DotItem<char>(Parameters, 0, "\'+\'");
        CERBLIB_UNUSED(auto) = lex::DotItem<char>(Parameters, 1, "\"Hello, World!\"");
        CERBLIB_UNUSED(auto) = lex::DotItem<char>(Parameters, 2, "\"for\"p+[a-z]*");
        CERBLIB_UNUSED(auto) = lex::DotItem<char>(Parameters, 3, "(\"for\"p*)+[a-z]+");

        testDotItemCreationOnErrorCases();
        return 0;
    }
}// namespace cerb::debug
