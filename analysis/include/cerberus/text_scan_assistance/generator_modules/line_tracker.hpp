#ifndef CERBERUS_LINE_TRACKER_HPP
#define CERBERUS_LINE_TRACKER_HPP

#include <cerberus/char.hpp>
#include <cerberus/string_view.hpp>

namespace cerb::text
{
    template<CharacterLiteral CharT, CharacterLiteral FileNameT = char>
    class GeneratorForText;
}

namespace cerb::text::gen
{
    template<CharacterLiteral CharT, CharacterLiteral FileNameT = char>
    class LineWatcher
    {
        using char_enum = lex::CharEnum<CharT>;
        using generator_t = GeneratorForText<CharT, FileNameT>;

    public:
        CERBLIB_DECL auto get() const -> BasicStringView<CharT> const &
        {
            return line;
        }

        constexpr auto update(generator_t const &gen) -> void
        {
            if (isUpdateNeeded(gen)) {
                updateLine(gen);
            }
        }

        constexpr auto forceUpdate(generator_t const &gen) -> void
        {
            updateLine(gen);
        }

        LineWatcher() = default;

    private:
        CERBLIB_DECL auto isUpdateNeeded(generator_t const &gen) const -> bool
        {
            auto previous_offset = gen.getOffset() - 1;
            return gen.at(previous_offset) == char_enum::NewLine;
        }

        constexpr auto updateLine(generator_t const &gen) -> void
        {
            auto const &text = gen.text;
            auto offset = gen.getOffset();

            size_t line_end = text.find(char_enum::NewLine, offset);
            size_t line_length = line_end - offset;

            line = { gen.currentBegin(), line_length };
        }

        BasicStringView<CharT> line{};
    };
}// namespace cerb::text::gen

#endif /* CERBERUS_LINE_TRACKER_HPP */
