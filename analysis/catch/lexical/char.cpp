#include "char.hpp"
#include <cerberus/lexical/char.hpp>

namespace cerb::test
{
    auto endOfInputTest() -> void
    {
        using namespace lex;

        for (u16 i = 1; i <= 255; ++i) {
            auto chr = static_cast<u8>(i);
            EXPECT_FALSE(isEoF(chr));
            EXPECT_FALSE(isEoF(std::bit_cast<char>(chr)));
        }
    }

    auto lexicalCharTest(u32) -> int
    {
        endOfInputTest();
        return 0;
    }
}// namespace cerb::test
