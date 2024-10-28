#include <exception>
#include <stdexcept>

class S
{
    bool has_error() const;

public:
    ~S() noexcept(false) // UndCC_Violation
    {
        // Normal processing
        if (has_error() && !std::uncaught_exception())
        {
            throw std::logic_error("Something bad");
        }
    }
};