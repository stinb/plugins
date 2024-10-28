#include <string>
#include <optional>
#include <cstdint>

std::string getKey(std::string const &key_data)
{
    errno = 42;     // UndCC_Violation - non-zero value
    errno = EINVAL; // UndCC_Violation - does not expand to literal '0'
    return std::string{};
}

#define OK 0

void errnoSettingFunction() {}
void handleError() {}

void f()
{
    uint32_t success{0};
    errno = success; // UndCC_Violation - must use literal '0'
    errno = OK;      // UndCC_Valid - 'OK' expands to literal '0'
    errnoSettingFunction();
    if (errno != success)
    {
        handleError();
    }
}
