#include <cerberus/debug/debug.hpp>
#include <cerberus/text_scan_assistance/bracket_finder.hpp>

namespace cerb::debug
{
    using namespace lex;
    using namespace text;

    static constexpr string_view TestInput = "(Hello, World())!";
    static constexpr u16string_view TestInputU16 = u"(Hello, World())!";

    static constexpr string_view EmptyTestInput = "()";
    static constexpr u16string_view EmptyTestInputU16 = u"()";

    static constexpr string_view ErroneousTestInput = "((Hello, World())!";
    static constexpr u16string_view ErroneousTestInputU16 = u"((Hello, World())!";

    CERBERUS_TEST_FUNC_WITH_CONSTEXPR_STRING(testBracketFinderOnBasicString)
    {
        constexpr auto last_bracket = TestInput.rfind(')');

        return findBracket('(', ')', GeneratorForText(TestInput)) == last_bracket and
               findBracket('(', ')', GeneratorForText(EmptyTestInput)) == 1;
    }

    CERBERUS_TEST_FUNC_WITH_CONSTEXPR_STRING(testBracketFinderOnU16String)
    {
        constexpr auto last_bracket = TestInputU16.rfind(u')');

        ASSERT_EQUAL(findBracket(u'(', u')', GeneratorForText(TestInputU16)), last_bracket);
        ASSERT_EQUAL(findBracket(u'(', u')', GeneratorForText(EmptyTestInputU16)), 1);

        return true;
    }

    auto testBracketFinderOnErrorCase() -> bool
    {
        ERROR_EXPECTED(
            CERBLIB_UNUSED(auto) = findBracket('(', ')', GeneratorForText(ErroneousTestInput)),
            BasicBracketFinderError,
            "Analysis error occurred: Unexpected EoF! File: , line: 1, char: 19\n"
            "((Hello, World())!\n"
            "                  ^")

        return true;
    }

    auto testBracketFinderOnErrorCaseU16() -> bool
    {
        ERROR_EXPECTED(
            CERBLIB_UNUSED(auto) = findBracket(u'(', u')', GeneratorForText(ErroneousTestInputU16)),
            BasicBracketFinderError,
            "Analysis error occurred: Unexpected EoF! File: , line: 1, char: 19\n"
            "((Hello, World())!\n"
            "                  ^\n"
            "Warning! Character type is Ds instead of c, so error message might have some defects. "
            "Please use getMessage() instead.")

        return true;
    }

    auto testBracketFinder() -> int
    {
        CERBERUS_TEST_STD_STRING(testBracketFinderOnBasicString());
        CERBERUS_TEST_STD_STRING(testBracketFinderOnU16String());

        ASSERT_TRUE(testBracketFinderOnErrorCase());
        ASSERT_TRUE(testBracketFinderOnErrorCaseU16());
        return 0;
    }
}// namespace cerb::debug