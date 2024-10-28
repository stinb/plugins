#include <cstdint>

int32_t bar();
int32_t ga = 0; // UndCC_Valid - but violates "See also"

int32_t foo()
{
    int32_t a = 0;                  // UndCC_Valid
    static int32_t b = 0;           // UndCC_Violation
    static constexpr int32_t c = 0; // UndCC_Valid
    static const int32_t d = bar(); // UndCC_Valid
}

class Application
{
    static Application &theApp()
    {
        static Application app; // UndCC_Violation
        return app;
    }
};
