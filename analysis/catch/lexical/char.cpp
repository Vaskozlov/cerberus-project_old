#include "char.hpp"
#include <cerberus/lexical/char.hpp>

namespace cerb::test {
    auto endOfInputTest() -> void
    {
        EXPECT_TRUE(lex::isEndOfInput('\0'));
        EXPECT_TRUE(lex::isEndOfInput(u8'\0'));
        EXPECT_TRUE(lex::isEndOfInput(u'\0'));
        EXPECT_TRUE(lex::isEndOfInput(L'\0'));

        for (u16 i = 1; i <= 255; ++i) {
            auto chr = static_cast<u8>(i);
            EXPECT_FALSE(lex::isEndOfInput(chr));
            EXPECT_FALSE(lex::isEndOfInput(std::bit_cast<char>(chr)));
        }
    }

    auto lexicalCharTest(u32) -> int
    {
        endOfInputTest();
        return 0;
    }
}// namespace cerb::test
