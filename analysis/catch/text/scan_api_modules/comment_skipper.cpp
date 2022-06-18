#include <cerberus/debug/debug.hpp>
#include <cerberus/text_scan_assistance/scan_api.hpp>

namespace cerb::debug
{
    using namespace text;

    constexpr BasicStringView<char> TestInputSingleLine =
        "Hello, // This is a single line comment\n"
        "World!";

    constexpr BasicStringView<char> TestInputMultiline =
        "Hello, /* This is a multiline comment */\n"
        "World!";

    constexpr BasicStringView<char16_t> TestInputSingleLineUtf16 =
        u"Hello, // This is a single line comment\n"
        u"World!";

    constexpr BasicStringView<char16_t> TestInputMultilineUtf16 =
        u"Hello, /* This is a multiline comment */\n"
        u"World!";

    constexpr BasicStringView<char> ExpectedOutputSingleLine = "Hello, \nWorld!";
    constexpr BasicStringView<char> ExpectedOutputMultiline = "Hello, \nWorld!";

    constexpr BasicStringView<char16_t> ExpectedOutputSingleLineUtf16 = u"Hello, \nWorld!";
    constexpr BasicStringView<char16_t> ExpectedOutputMultilineUtf16 = u"Hello, \nWorld!";

    template<CharacterLiteral CharT>
    struct CommentSkipperTester : ScanApi<CharT>
    {
        CERBLIB_SCAN_API_ACCESS(CharT);

        constexpr CommentSkipperTester(
            BasicStringView<CharT> const &text,
            BasicStringView<CharT> const &expected,
            BasicStringView<CharT> const &single_line_comment = {},
            BasicStringView<CharT> const &multiline_comment_begin = {},
            BasicStringView<CharT> const &multiline_comment_end = {})
          : scan_api_t(
                generator,
                RAW_CHARS,
                single_line_comment,
                multiline_comment_begin,
                multiline_comment_end),
            generator(text), expected_text(expected)
        {
            scan_api_t::beginScanning(lex::CharEnum<CharT>::EoF);
        }

    private:
        constexpr auto onStart() -> ScanApiStatus override
        {
            return ScanApiStatus::DO_NOT_SKIP_CHAR;
        }

        constexpr auto processChar(CharT chr) -> void override
        {
            ASSERT_EQUAL(chr, expected_text[index]);
            ++index;
        }

        GeneratorForText<CharT> generator{};
        BasicStringView<CharT> expected_text{};
        size_t index{};
    };

    auto testCommentSkipperOnBasicStringSingleLine() -> bool
    {
        CommentSkipperTester<char> tester{ TestInputSingleLine, ExpectedOutputSingleLine, "//",
                                           "/*", "*/" };
        return true;
    }

    auto testCommentSkipperOnBasicStringMultiline() -> bool
    {
        CommentSkipperTester<char> tester{ TestInputMultiline, ExpectedOutputMultiline, "//", "/*",
                                           "*/" };
        return true;
    }

    auto testCommentSkipperOnBasicStringSingleLineUtf16() -> bool
    {
        CommentSkipperTester<char16_t> tester{ TestInputSingleLineUtf16,
                                               ExpectedOutputSingleLineUtf16, u"//", u"/*", u"*/" };
        return true;
    }

    auto testCommentSkipperOnBasicStringMultilineUtf16() -> bool
    {
        CommentSkipperTester<char16_t> tester{ TestInputMultilineUtf16,
                                               ExpectedOutputMultilineUtf16, u"//", u"/*", u"*/" };
        return true;
    }

    auto testCommentSkipper() -> int
    {
        ASSERT_TRUE(testCommentSkipperOnBasicStringSingleLine());
        ASSERT_TRUE(testCommentSkipperOnBasicStringMultiline());

        ASSERT_TRUE(testCommentSkipperOnBasicStringSingleLineUtf16());
        ASSERT_TRUE(testCommentSkipperOnBasicStringMultilineUtf16());
        return 0;
    }
}// namespace cerb::debug
