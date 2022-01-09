#ifndef CERBERUS_DOT_ITEM_HPP
#define CERBERUS_DOT_ITEM_HPP

#include <cerberus/bitmap.hpp>
#include <cerberus/lexical/sequence.hpp>
#include <cerberus/lexical/string_parser.hpp>
#include <cerberus/lexical/string_sequence.hpp>
#include <cerberus/lexical/token.hpp>

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
        using string_sequence = StringSequence<CharT>;
        using Flags = typename sequence::Flags;
        using Object = DotItemObject<CharT>;
        using storage_t = std::vector<std::unique_ptr<Object>>;

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

        CERBLIB_DECL auto getLastSequence() -> Object &
        {
            if (objects.empty()) {
                throw DotItemParsingError("Unable to find an sequences");
            }

            return *objects.back().get();
        }

        CERBLIB_DECL auto getLastSequence() const -> Object &
        {
            if (objects.empty()) {
                throw DotItemParsingError("Unable to find an sequences");
            }

            return *objects.back().get();
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
            std::unique_ptr<Object> new_sequence = std::make_unique<sequence>(
                flags_for_sequence, iterator_for_text, text_generator.end());

            iterator_for_text = new_sequence->getIterator();
            objects.push_back(std::move(new_sequence));
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
        {
            std::unique_ptr<Object> new_string_sequence = std::make_unique<string_sequence>(
                flags_for_sequence, iterator_for_text, text_generator.end());

            iterator_for_text = new_string_sequence->getIterator();
            objects.push_back(std::move(new_string_sequence));
        }

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
        storage_t objects{};
        Flags flags_for_sequence{};
    };
}// namespace cerb::lex

#endif /* CERBERUS_DOT_ITEM_HPP */
