// https://releases.llvm.org/18.1.0/tools/clang/tools/extra/docs/clang-tidy/checks/cppcoreguidelines/avoid-non-const-global-variables.html

char a;  // UndCC_Violation
const char b =  0;

namespace some_namespace
{
    char c;  // UndCC_Violation
    const char d = 0;
}

char * c_ptr1 = &some_namespace::c;  // UndCC_Violation
char *const c_const_ptr = &some_namespace::c;  // UndCC_Violation
char & c_reference = some_namespace::c;  // UndCC_Violation

class Foo  // No Warnings inside Foo, only namespace scope is covered
{
public:
    char e = 0;
    const char f = 0;
protected:
    char g = 0;
private:
    char h = 0;
};
