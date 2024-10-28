#include <cstdint>
#include <memory>
#include <vector>
std::int8_t AppMainLoop() noexcept
{
    std::int8_t retCode = 0;
    std::int32_t* arr[10];
    while (true)
    {
        for (std::int8_t i = 0; i < 10; ++i)
        {
            arr[i] = new std::int32_t{i}; // UndCC_Violation - allocation in a phase that requires real-time
        }
        // Implementation
        for (auto& i : arr)
        {
            delete i; // UndCC_Violation - deallocation in a phase that requires real-time
        }
    }
    return retCode;
}
static std::int32_t* object = new std::int32_t{0}; // UndCC_Valid - allocating in start-up phase

int main(int, char**)
{
    std::unique_ptr<std::int32_t> ptr = std::make_unique<std::int32_t>(0); // UndCC_Valid
    std::vector<std::int32_t> vec; // UndCC_Valid
    vec.reserve(10); // UndCC_Valid

    std::int8_t code = AppMainLoop();
    return code;
}
