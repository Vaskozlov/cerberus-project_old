#ifndef CERBERUS_LEXICAL_EXCEPTIONS_HPP
#define CERBERUS_LEXICAL_EXCEPTIONS_HPP

#include <cerberus/exception.hpp>

namespace cerb::lex
{
    CERBERUS_EXCEPTION(ParsingError);
    CERBERUS_EXCEPTION(SequenceError);
}// namespace cerb::lex

#endif /* CERBERUS_LEXICAL_EXCEPTIONS_HPP */
