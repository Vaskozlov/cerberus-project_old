#include <cerberus/debug/debug.hpp>
#include <cerberus/lex/bracket_finder.hpp>

namespace cerb::debug
{
    using namespace lex;

    static constexpr BasicStringView<char> TestInput = "(Hello, World())!";
    static constexpr BasicStringView<char> TestInputEmpty = "()";

    static constexpr BasicStringView<char16_t> TestInputU16 = u"(Hello, World())!";
    static constexpr BasicStringView<char16_t> TestInputEmptyU16 = u"()";

    static constexpr BasicStringView<char> TestErrorInput = "((Hello, World())!";

    static constexpr BasicStringView<char16_t> TestErrorInputU16 = u"((Hello, World())!";

    constexpr auto testBracketFinderOnBasicString() -> bool
    {
        return findBracket('(', ')', TestInput) == 15 and
               findBracket('(', ')', TestInputEmpty) == 1;
    }

    constexpr auto testBracketFinderOnU16String() -> bool
    {
        return findBracket(u'(', u')', TestInputU16) == 15 and
               findBracket(u'(', u')', TestInputEmptyU16) == 1;
    }

    constexpr auto testBracketFinderOnErrorCase() -> bool
    {
        try {
            findBracket('(', ')', TestErrorInput);
            CANT_BE_REACHED;
        } catch (BracketFinderError const &) {
            MUST_BE_REACHED;
        }

        return true;
    }

    constexpr auto testBracketFinderOnErrorCaseU16() -> bool
    {
        try {
            findBracket(u'(', u')', TestErrorInputU16);
            CANT_BE_REACHED;
        } catch (BracketFinderError const &) {
            MUST_BE_REACHED;
        }

        return true;
    }

    auto testBracketFinder() -> int
    {
        CERBERUS_TEST(testBracketFinderOnBasicString());
        CERBERUS_TEST(testBracketFinderOnU16String());

        EXPECT_TRUE(testBracketFinderOnErrorCase());
        EXPECT_TRUE(testBracketFinderOnErrorCaseU16());
        return 0;
    }
}// namespace cerb::debug
