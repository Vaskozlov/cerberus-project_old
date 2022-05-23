#include <cerberus/debug/debug.hpp>
#include <cerberus/text/bracket_finder.hpp>

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

    auto testBracketFinderOnBasicString() -> bool
    {
        constexpr auto last_bracket = 15;// TestInput.rfind(')') - 1;

        return findBracket('(', ')', GeneratorForText(TestInput)) == last_bracket and
               findBracket('(', ')', GeneratorForText(EmptyTestInput)) == 1;
    }

    auto testBracketFinderOnU16String() -> bool
    {
        return findBracket(u'(', u')', GeneratorForText(TestInputU16)) == 15 and
               findBracket(u'(', u')', GeneratorForText(EmptyTestInputU16)) == 1;
    }

    auto testBracketFinderOnErrorCase() -> bool
    {
        try {
            CERBLIB_UNUSED(auto) = findBracket('(', ')', GeneratorForText(ErroneousTestInput));
            CANT_BE_REACHED;
        } catch (BracketFinderError const &) {
            MUST_BE_REACHED;
        }

        return true;
    }

    auto testBracketFinderOnErrorCaseU16() -> bool
    {
        try {
            CERBLIB_UNUSED(auto) = findBracket(u'(', u')', GeneratorForText(ErroneousTestInputU16));
            CANT_BE_REACHED;
        } catch (BracketFinderError const &) {
            MUST_BE_REACHED;
        }

        return true;
    }

    auto testBracketFinder() -> int
    {
        EXPECT_TRUE(testBracketFinderOnBasicString());
        EXPECT_TRUE(testBracketFinderOnU16String());

        EXPECT_TRUE(testBracketFinderOnErrorCase());
        EXPECT_TRUE(testBracketFinderOnErrorCaseU16());
        return 0;
    }
}// namespace cerb::debug
