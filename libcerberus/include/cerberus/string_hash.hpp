#ifndef CERBERUS_STRING_HASH_HPP
#define CERBERUS_STRING_HASH_HPP

#include <cerberus/string_view.hpp>

namespace cerb::hash
{
    template<CharacterLiteral CharT>
    struct StringHash
    {
        CERBLIB_DECL auto operator()() const -> size_t
        {
            return hash;
        }

        StringHash() = default;

        template<StringType<CharT> Str>// NOLINTNEXTLINE
        consteval StringHash(Str const &str)
        {
            calculateHash(str);
        }

        // NOLINTNEXTLINE
        consteval StringHash(BasicStringView<CharT> const &str)
        {
            calculateHash(str);
        }

        template<StringType<CharT> Str>
        constexpr StringHash(bool /*runtime*/, Str const &str)
        {
            calculateHash(str);
        }

        constexpr StringHash(bool /*runtime*/, BasicStringView<CharT> const &str)
        {
            calculateHash(str);
        }

    private:
        template<StringType<CharT> Str>
        constexpr auto calculateHash(Str const &str) -> void
        {
            constexpr size_t hash_multiplier = 31;

            for (CharT chr : str) {
                hash *= hash_multiplier;
                hash += asUInt(chr);
            }
        }

        size_t hash{};
    };

    template<CharacterLiteral CharT>
    CERBLIB_DECL auto hashString(std::basic_string<CharT> const &str) -> size_t
    {
        return StringHash<CharT>{ true, str }();
    }

    template<CharacterLiteral CharT>
    CERBLIB_DECL auto hashString(BasicStringView<CharT> const &str) -> size_t
    {
        return StringHash<CharT>{ true, str }();
    }
}// namespace cerb::hash

#endif /* CERBERUS_STRING_HASH_HPP */
