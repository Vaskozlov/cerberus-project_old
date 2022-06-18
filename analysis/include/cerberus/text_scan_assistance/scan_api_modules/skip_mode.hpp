#ifndef CERBERUS_SKIP_MODE_HPP
#define CERBERUS_SKIP_MODE_HPP

#include <cerberus/enum.hpp>

namespace cerb::text
{
    enum SkipMode : u8
    {
        CLEAN_CHARS,
        RAW_CHARS
    };
}// namespace cerb::text

#endif /* CERBERUS_SKIP_MODE_HPP */
