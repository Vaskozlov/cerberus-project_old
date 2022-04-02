#include <cerberus/debug/debug.hpp>
#include <cerberus/lex/item/item.hpp>

namespace cerb::debug
{
    auto testDotItem() -> int
    {
        lex::AnalysisGlobals<char> parameters{};

        CERBLIB_UNUSED(auto) = lex::DotItem<char>(0, "\'+\'", parameters);
        CERBLIB_UNUSED(auto) = lex::DotItem<char>(1, "(\"for\"p*)+[a-z]+", parameters);

        return 0;
    }
}// namespace cerb::debug
