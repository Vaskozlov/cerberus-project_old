#include "string_parser.hpp"
#include <cerberus/lexical/string_parser.hpp>

namespace cerb::test {
    auto stringParserTest(u32) -> int
    {
        cerb::string_view string = "\"test \\xff\\n\"";

        auto begin_of_string = string.begin();
        auto end_of_string = string.end();

        lex::StringParser string_parser{ '\"', begin_of_string, end_of_string };

        EXPECT_TRUE(string_parser.parseString() == "test \xff\n");

        cerb::u16string_view string_16 = u"\"test \\uFFFF\\xff\\n\"";
        auto begin_of_string_16 = string_16.begin();
        auto end_of_string_16 = string_16.end();

        lex::StringParser string_parser_16{ u'\"', begin_of_string_16, end_of_string_16 };

        EXPECT_TRUE(string_parser_16.parseString() == u"test \uFFFF\xff\n");
        return 0;
    }
}// namespace cerb::test
