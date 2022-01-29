#include "string_parser.hpp"
#include <cerberus/lexical/string_parser.hpp>
#include <cerberus/lexical/text_generator.hpp>

namespace cerb::test
{
    auto testStringParserOnChar() -> void
    {
        cerb::string_view string = "\"test \\xff\\n\"";
        lex::TextGenerator generator{ string };
        generator.getRawChar();
        lex::StringParser string_parser{ '\"', generator };

        EXPECT_TRUE(string_parser.parseString() == "test \xff\n");
    }

    auto testStringParserOnChar16() -> void
    {
        cerb::u16string_view string = u"\"test \\uFFFF\\xff\\n\"";
        lex::TextGenerator generator{ string };
        generator.getRawChar();
        lex::StringParser string_parser{ u'\"', generator };

        EXPECT_TRUE(string_parser.parseString() == u"test \uFFFF\xff\n");
    }

    auto stringParserTest(u32) -> int
    {
        testStringParserOnChar();
        testStringParserOnChar16();
        return 0;
    }
}// namespace cerb::test
