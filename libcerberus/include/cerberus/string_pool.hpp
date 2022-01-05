#ifndef CERBERUS_STRING_POOL_HPP
#define CERBERUS_STRING_POOL_HPP

#include <cerberus/string_view.hpp>
#include <map>

namespace cerb {
    template<CharacterLiteral CharT, typename TokenType>
    using StringPool = std::map<BasicStringView<CharT>, TokenType>;
}// namespace cerb

#endif /* CERBERUS_STRING_POOL_HPP */
