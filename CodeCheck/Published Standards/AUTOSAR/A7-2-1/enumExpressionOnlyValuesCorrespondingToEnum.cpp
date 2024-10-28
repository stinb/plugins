// $Id: A7-2-1.cpp 289436 2017-10-04 10:45:23Z michal.szczepankiewicz $
#include <cstdint>

enum class E : std::uint8_t
{
    Ok = 0,
    Repeat,
    Error
};

E Convert1(std::uint8_t number) noexcept
{
    E result = E::Ok; // UndCC_Valid
    switch (number)
    {
    case 0:
    {
        result = E::Ok; // UndCC_Valid
        break;
    }
    case 1:
    {
        result = E::Repeat; // UndCC_Valid
        break;
    }
    case 2:
    {
        result = E::Error; // UndCC_Valid
        break;
    }
    case 3:
    {
        constexpr std::int8_t val = 3;
        result = static_cast<E>(val); // UndCC_Violation - value 3 does not
        // correspond to any of E’s
        // enumerators
        break;
    }
    default:
    {
        result =
            static_cast<E>(0); // UndCC_Valid - value 0 corresponds to E::Ok
        break;
    }
    }
    return result;
}

E Convert2(std::uint8_t userInput) noexcept
{
    E result = static_cast<E>(userInput); 
    // userInput may not correspond to
    // any of E’s enumerators
    return result;
}

E Convert3(std::uint8_t userInput) noexcept
{
    E result = E::Error;
    if (userInput <= 2)
    {
        result = static_cast<E>(userInput); // UndCC_Valid - the range of
        // userInput checked before casting
        // it to E enumerator
    }
    return result;
}
