#include <cerberus/analysis/analysis_exception.hpp>

namespace cerb::analysis
{
    template struct AnalysisException<char>;
    template struct AnalysisException<char8_t>;
    template struct AnalysisException<char16_t>;
    template struct AnalysisException<char32_t>;
    template struct AnalysisException<wchar_t>;
}// namespace cerb::analysis
