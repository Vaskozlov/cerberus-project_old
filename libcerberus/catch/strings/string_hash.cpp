#include <cerberus/debug/debug.hpp>
#include <cerberus/string_hash.hpp>

namespace cerb::debug
{
    CERBERUS_TEST_FUNC(testHashStringWithBasicStringView)
    {
        hash::StringHash<char> zeroStringHash("\0");
        ASSERT_EQUAL(zeroStringHash(), 0);

        hash::StringHash<char> singleCharStringHash("\x09");
        ASSERT_EQUAL(singleCharStringHash(), 9);

        hash::StringHash<char> multiCharStringHash("\x0A\x09");
        ASSERT_EQUAL(multiCharStringHash(), 319);

        return true;
    }

    CERBERUS_TEST_FUNC(testHashStringWithUtf16StringView)
    {
        hash::StringHash<char16_t> zeroStringHash(u"\0");
        ASSERT_EQUAL(zeroStringHash(), 0);

        hash::StringHash<char16_t> singleCharStringHash(u"\x09");
        ASSERT_EQUAL(singleCharStringHash(), 9);

        hash::StringHash<char16_t> multiCharStringHash(u"\x0A\x09");
        ASSERT_EQUAL(multiCharStringHash(), 319);

        return true;
    }

    CERBERUS_TEST_FUNC_WITH_CONSTEXPR_STRING(testHashStringWithBasicString)
    {
        using namespace std::string_literals;

        hash::StringHash<char> zeroStringHash(true, "\0"s);
        ASSERT_EQUAL(zeroStringHash(), 0);

        hash::StringHash<char> singleCharStringHash(true, "\x09"s);
        ASSERT_EQUAL(singleCharStringHash(), 9);

        hash::StringHash<char> multiCharStringHash(true, "\x0A\x09"s);
        ASSERT_EQUAL(multiCharStringHash(), 319);

        return true;
    }

    CERBERUS_TEST_FUNC_WITH_CONSTEXPR_STRING(testHashStringWithUtf16String)
    {
        using namespace std::string_literals;

        hash::StringHash<char16_t> zeroStringHash(true, u"\0"s);
        ASSERT_EQUAL(zeroStringHash(), 0);

        hash::StringHash<char16_t> singleCharStringHash(true, u"\x09"s);
        ASSERT_EQUAL(singleCharStringHash(), 9);

        hash::StringHash<char16_t> multiCharStringHash(true, u"\x0A\x09"s);
        ASSERT_EQUAL(multiCharStringHash(), 319);

        return true;
    }

    auto testHashString() -> int
    {
        CERBERUS_TEST(testHashStringWithBasicStringView());
        CERBERUS_TEST(testHashStringWithUtf16StringView());

        CERBERUS_TEST_FOR_CONSTEXPR_STRING(testHashStringWithBasicString());
        CERBERUS_TEST_FOR_CONSTEXPR_STRING(testHashStringWithUtf16String());
        return 0;
    }
}// namespace cerb::debug
