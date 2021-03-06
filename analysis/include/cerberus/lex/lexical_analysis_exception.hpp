#ifndef CERBERUS_LEXICAL_ANALYSIS_EXCEPTION_HPP
#define CERBERUS_LEXICAL_ANALYSIS_EXCEPTION_HPP

#include <cerberus/analysis/analysis_basic_exception.hpp>

namespace cerb::lex
{
    CERBERUS_EXCEPTION(BasicLexicalAnalysisException, analysis::BasicAnalysisException);

    template<CharacterLiteral CharT>
    CERBERUS_EXCEPTION(LexicalAnalysisException, BasicLexicalAnalysisException);
}// namespace cerb::lex

#endif /* CERBERUS_LEXICAL_ANALYSIS_EXCEPTION_HPP */
