#include <memory>

struct S
{
};

void f()
{
    std::unique_ptr<S> s{new S[10]};    // UndCC_Violation
}
