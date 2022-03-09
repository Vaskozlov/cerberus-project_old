#ifndef CERBERUS_BIT_PATTERN_HPP
#define CERBERUS_BIT_PATTERN_HPP

#include <cerberus/cerberus.hpp>

namespace cerb
{
    template<typename T>
    class CERBLIB_TRIVIAL BitPattern
    {
        static_assert(std::is_integral_v<T>);

        T m_expected{};
        T m_mask{ ~static_cast<T>(0) };

    public:
        constexpr auto operator==(T const &value) const -> bool
        {
            return (value & m_mask) == m_expected;
        }

        constexpr auto operator!=(T const &value) const -> bool
        {
            return !operator==(value);
        }

        consteval explicit BitPattern(std::string_view const &input)
        {
            T current_bit = (static_cast<T>(1) << (input.size() - 2));

            for (char elem : input) {
                if (elem == '\0') {
                    return;
                }
                if (elem == '1') {
                    m_expected |= current_bit;
                } else if (logicalOr(elem == 'x', elem == 'X')) {
                    m_mask &= ~current_bit;
                }
                current_bit >>= 1U;
            }
        }
    };
}// namespace cerb

#endif /* CERBERUS_BIT_PATTERN_HPP */
