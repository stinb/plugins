#include <memory>

struct S
{
};

void f()
{
    std::unique_ptr<S[]> s = std::make_unique<S[]>(10);
}
