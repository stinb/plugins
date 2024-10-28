#define FUNC(X) ((X) + (X)) // UndCC_Violation

template <typename T>
constexpr auto func(T x) // Possible alternative
{
    return x + x;
}

// The following examples are compliant by exception:
#define ID(name) constexpr auto name = #name; // UndCC_Violation(exc) - use of #

#define TAG(name) class name##Tag{}; // UndCC_Violation(exc) - use of ##

#define LOG(message) log(__func__, message); // // UndCC_Violation(exc) - use of __func__
