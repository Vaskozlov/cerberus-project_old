#ifndef CERBERUS_BITMAP_HPP
#define CERBERUS_BITMAP_HPP

#include <array>
#include <memory>

namespace cerb {
    enum struct ValueOfBit
    {
        ONE,
        ZERO,
        ANY
    };

    template<size_t AxisN, size_t BitN>
    struct ConstBitmap
    {
        static_assert(AxisN != 0);

        using value_type       = usize;
        using const_value_type = value_type;
        using pointer          = value_type *;
        using const_pointer    = const value_type *;

        constexpr static size_t length_of_axis =
            BitN / bitsizeof(value_type) + (BitN % bitsizeof(value_type) != 0);

        using axis_storage_t = std::array<value_type, length_of_axis>;
        using storage_t      = std::array<axis_storage_t, AxisN>;

    private:
        storage_t m_data{};

    public:
        CERBLIB_DECL auto axis() const -> size_t
        {
            return AxisN;
        }

        CERBLIB_DECL auto size() const -> size_t
        {
            return BitN;
        }

        CERBLIB_DECL auto lengthOfAxis() const -> size_t
        {
            return length_of_axis;
        }

        CERBLIB_DECL auto axisSize() const -> size_t
        {
            return sizeof(axis_storage_t);
        }

        CERBLIB_DECL auto storageSize() const -> size_t
        {
            return sizeof(storage_t);
        }

        template<size_t Axis>
        CERBLIB_DECL auto data() const -> pointer
        {
            static_assert(Axis <= AxisN);
            return m_data[Axis].data();
        }

        CERBLIB_DECL auto storage() const -> const storage_t &
        {
            return m_data;
        }

        template<size_t Axis>
        CERBLIB_DECL auto axisStorage() const -> const axis_storage_t &
        {
            static_assert(Axis <= AxisN);
            return m_data[Axis];
        }

        constexpr ConstBitmap() = default;
    };

}// namespace cerb

#endif /* CERBERUS_BITMAP_HPP */
