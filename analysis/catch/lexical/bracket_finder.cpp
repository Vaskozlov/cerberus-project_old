#include <cerberus/debug/debug.hpp>
#include <cerberus/lex/bracket_finder.hpp>

namespace cerb::debug
{
    using namespace lex;

    static constexpr string_view TestInput = "(Hello, World())!";
    static constexpr string_view TestInputEmpty = "()";

    static constexpr u16string_view TestInputU16 = u"(Hello, World())!";
    static constexpr u16string_view TestInputEmptyU16 = u"()";

    static constexpr string_view TestErrorInput = "((Hello, World())!";

    static constexpr u16string_view TestErrorInputU16 = u"((Hello, World())!";

    auto testBracketFinderOnBasicString() -> bool
    {
        return findBracket('(', ')', GeneratorForText(TestInput)) == 15 and
               findBracket('(', ')', GeneratorForText(TestInputEmpty)) == 1;
    }

    auto testBracketFinderOnU16String() -> bool
    {
        return findBracket(u'(', u')', GeneratorForText(TestInputU16)) == 15 and
               findBracket(u'(', u')', GeneratorForText(TestInputEmptyU16)) == 1;
    }

    auto testBracketFinderOnErrorCase() -> bool
    {
        try {
            CERBLIB_UNUSED(auto) = findBracket('(', ')', GeneratorForText(TestErrorInput));
            CANT_BE_REACHED;
        } catch (BracketFinderError const &) {
            MUST_BE_REACHED;
        }

        return true;
    }

    auto testBracketFinderOnErrorCaseU16() -> bool
    {
        try {
            CERBLIB_UNUSED(auto) =
                findBracket(u'(', u')', GeneratorForText(TestErrorInputU16));
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
