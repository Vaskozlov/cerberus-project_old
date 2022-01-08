#ifndef CERBERUS_DOT_ITEM_HPP
#define CERBERUS_DOT_ITEM_HPP

#include <cerberus/bitmap.hpp>
#include <cerberus/enum.hpp>
#include <cerberus/lexical/sequence.hpp>
#include <cerberus/lexical/token.hpp>
#include <deque>
#include <list>

namespace cerb::lex {
    template<CharacterLiteral CharT, typename TokenType>
    class DotItem
    {
        template<std::integral T>
        CERBLIB_DECL static auto cast(T value)
        {
            return static_cast<CharT>(value);
        }

        using str_view = BasicStringView<CharT>;
        using text_iterator = GetIteratorType<TextGenerator<CharT>>;
        using sequence = Sequence<CharT>;
        using sequence_flags = typename sequence::Flags;
        using sequence_parser = typename sequence::sequence_parser;
        using sequence_storage = std::deque<sequence>;

    public:
        explicit constexpr DotItem(str_view regex)
          : text_generator(LocationInFile("DotItem generator"), regex),
            iterator_for_text(text_generator.begin())
        {
            parseRegex();
        }

    private:
        constexpr auto clearFlags()
        {
            flags_for_sequence = sequence::Flags::NONE;
        }

        CERBLIB_DECL auto getLastSequence() -> sequence &
        {
            if (sequences.empty()) {
                throw DotItemParsingError("Unable to find an sequences");
            }

            return sequences.back();
        }

        CERBLIB_DECL auto getLastSequence() const -> sequence &
        {
            if (sequences.empty()) {
                throw DotItemParsingError("Unable to find an sequences");
            }

            return sequences.back();
        }

        constexpr auto parseRegex()
        {
            for (; iterator_for_text != text_generator.end(); ++iterator_for_text) {
                switch (*iterator_for_text) {
                case '[':
                    startNewSequenceOfChar();
                    endNewSequenceOfChar();
                    break;

                case 'p':
                    addPrefixOrPostfixTag();
                    break;

                case 'r':
                    addReverseTag();
                    break;

                case '"':
                    processString();
                    break;

                case '*':
                    addStar();
                    break;

                case '+':
                    addPlus();
                    break;

                case '?':
                    addQuestion();
                    break;

                default:
                    throw DotItemParsingError("Unable to do anything with given regex");
                }
            }
        }

        constexpr auto startNewSequenceOfChar() -> void
        {
            sequences.emplace_back(flags_for_sequence, iterator_for_text, text_generator.end());
            iterator_for_text = getLastSequence().getIterator();
        }

        constexpr auto endNewSequenceOfChar() -> void
        {
            clearFlags();
        }

        constexpr auto addPrefixOrPostfixTag() -> void
        {
            flags_for_sequence |= sequence::Flags::PREFIX_OR_POSTFIX;
        }

        constexpr auto addReverseTag() -> void
        {
            flags_for_sequence |= sequence::Flags::REVERSE;
        }

        constexpr auto processString() -> void
        {}

        constexpr auto addStar() -> void
        {
            getLastSequence().setRule(sequence::Rule::STAR);
        }

        constexpr auto addPlus() -> void
        {
            getLastSequence().setRule(sequence::Rule::PLUS);
        }

        constexpr auto addQuestion() -> void
        {
            getLastSequence().setRule(sequence::Rule::QUESTION);
        }

        TextGenerator<CharT> text_generator{};
        text_iterator iterator_for_text{};
        sequence_storage sequences{};
        sequence_flags flags_for_sequence{};
    };
}// namespace cerb::lex

#endif /* CERBERUS_DOT_ITEM_HPP */
