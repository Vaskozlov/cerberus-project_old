#include <cerberus/lex/analysis_exception.hpp>

namespace cerb
{
    template struct AnalysisException<char>;
    template struct AnalysisException<char8_t>;
    template struct AnalysisException<char16_t>;
}// namespace cerb
