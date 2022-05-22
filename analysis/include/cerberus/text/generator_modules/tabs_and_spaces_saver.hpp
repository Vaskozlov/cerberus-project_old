#ifndef CERBERUS_TABS_AND_SPACES_SAVER_HPP
#define CERBERUS_TABS_AND_SPACES_SAVER_HPP

#include <cerberus/lex/char.hpp>
#include <string>

namespace cerb::text::gen
{
    template<CharacterLiteral CharT>
    struct TabsAndSpacesSaver
    {
        using char_enum = lex::CharEnum<CharT>;

        CERBLIB_DECL auto get() const -> std::basic_string<CharT> const &
        {
            return tabs_and_spaces;
        }

        constexpr auto clear() -> void
        {
            tabs_and_spaces.clear();
        }

        CERBLIB_DECL auto empty() const -> bool
        {
            return tabs_and_spaces.empty();
        }

        constexpr auto tryToClear(CharT chr) -> void
        {
            current_char = chr;

            if (needToClear()) {
                clear();
            }
        }

        constexpr auto tryToAdd(CharT chr) -> void
        {
            current_char = chr;

            if (isTabOrSpace()) {
                tabs_and_spaces.push_back(current_char);
            }
        }

        TabsAndSpacesSaver() = default;

    private:
        CERBLIB_DECL auto needToClear() const -> bool
        {
            return logicalOr(
                current_char == char_enum::NewLine, logicalAnd(not empty(), not isTabOrSpace()));
        }

        CERBLIB_DECL auto isTabOrSpace() const -> bool
        {
            return logicalOr(current_char == char_enum::Tab, current_char == char_enum::Space);
        }

        std::basic_string<CharT> tabs_and_spaces{};
        CharT current_char{};
    };

#ifndef CERBERUS_HEADER_ONLY
    extern template struct TabsAndSpacesSaver<char>;
    extern template struct TabsAndSpacesSaver<char8_t>;
    extern template struct TabsAndSpacesSaver<char16_t>;
    extern template struct TabsAndSpacesSaver<char32_t>;
    extern template struct TabsAndSpacesSaver<wchar_t>;
#endif /* CERBERUS_HEADER_ONLY */
}// namespace cerb::text::gen

#endif /* CERBERUS_TABS_AND_SPACES_SAVER_HPP */
