// $Id: A13-5-4.cpp 328319 2018-08-03 14:08:42Z christof.meerwald $
#include <cstdint>


class A // UndCC_Violation
{
    public:
        explicit A(std::uint32_t d) : d(d) {}

        friend bool operator==(A const & lhs, A const & rhs) noexcept
        {
            return lhs.d == rhs.d;
        }
        friend bool operator!=(A const & lhs, A const & rhs) noexcept
        {
            return lhs.d != rhs.d;
        }

    private:
        std::uint32_t d;
};

// UndCC_Valid
class B
{
    public:
        explicit B(std::uint32_t d) : d(d) {}

        friend bool operator==(B const & lhs, B const & rhs) noexcept
        {
            return lhs.d == rhs.d;
        }

        friend bool operator!=(B const & lhs, B const & rhs) noexcept
        {
            return !(lhs == rhs);
        }
    private:
        std::uint32_t d;
};


class C // UndCC_Violation
{
    public:
        explicit C(std::uint32_t d) : d(d) {}

        friend bool operator<(C const & lhs, C const & rhs) noexcept
        {
            return lhs.d < rhs.d;
        }
        friend bool operator>=(C const & lhs, C const & rhs) noexcept
        {
            return lhs.d >= rhs.d;
        }

    private:
        std::uint32_t d;
};

// UndCC_Valid
class D
{
    public:
        explicit D(std::uint32_t d) : d(d) {}

        friend bool operator<(D const & lhs, D const & rhs) noexcept
        {
            return lhs.d < rhs.d;
        }
        friend bool operator>=(D const & lhs, D const & rhs) noexcept
        {
            return !(lhs < rhs);
        }

    private:
        std::uint32_t d;
};


class E // UndCC_Violation
{
    public:
        explicit E(std::uint32_t d) : d(d) {}

        friend bool operator>(E const & lhs, E const & rhs) noexcept
        {
            return lhs.d > rhs.d;
        }
        friend bool operator<=(E const & lhs, E const & rhs) noexcept
        {
            return lhs.d <= rhs.d;
        }

    private:
        std::uint32_t d;
};

// UndCC_Valid
class F
{
    public:
        explicit F(std::uint32_t d) : d(d) {}

        friend bool operator>(F const & lhs, F const & rhs) noexcept
        {
            return lhs.d > rhs.d;
        }
        friend bool operator<=(F const & lhs, F const & rhs) noexcept
        {
            return !(lhs > rhs);
        }

    private:
        std::uint32_t d;
};
