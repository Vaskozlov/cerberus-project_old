#ifndef CERBERUS_TABS_AND_SPACES_SAVER_HPP
#define CERBERUS_TABS_AND_SPACES_SAVER_HPP

#include <cerberus/type.hpp>
#include <string>

namespace cerb::text::gen
{
    template<CharacterLiteral CharT, CharacterLiteral FileNameT>
    struct TabsAndSpacesSaver
    {
        using char_enum = lex::CharEnum<CharT>;

        constexpr auto get() const -> std::basic_string<CharT> const &
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

        constexpr auto tryToClearTabsAndSpaces(CharT chr) -> void
        {
            current_char = chr;

            if (needToClearTabsAndSpaces()) {
                clear();
            }
        }

        constexpr auto tryToAddTabOrSpace(CharT chr) -> void
        {
            using namespace lex;
            current_char = chr;

            if (isTabOrSpace()) {
                tabs_and_spaces.push_back(current_char);
            }
        }

        TabsAndSpacesSaver() = default;

    private:
        CERBLIB_DECL auto needToClearTabsAndSpaces() const -> bool
        {
            using namespace lex;

            return logicalOr(
                current_char == char_enum::NewLine, logicalAnd(not empty(), not isTabOrSpace()));
        }

        CERBLIB_DECL auto isTabOrSpace() const -> bool
        {
            using namespace lex;

            return logicalOr(current_char == char_enum::Tab, current_char == char_enum::Space);
        }

        std::basic_string<CharT> tabs_and_spaces{};
        CharT current_char{};
    };
}// namespace cerb::text::gen

#endif /* CERBERUS_TABS_AND_SPACES_SAVER_HPP */
