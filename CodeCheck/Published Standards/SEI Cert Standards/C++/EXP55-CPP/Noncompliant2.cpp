#include <iostream>

class S
{
    int cachedValue;

    int compute_value() const; // expensive
public:
    S() : cachedValue(0) {}

    // ...
    int get_value() const
    {
        if (!cachedValue)
        {
            const_cast<S *>(this)->cachedValue = compute_value();
        }
        return cachedValue;
    }
};

void f()
{
    const S s;
    std::cout << s.get_value() << std::endl; // UndCC_Violation
}