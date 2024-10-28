#include <iostream>

extern "C" int func();

// the definition of func is written in assembly language
// raw string literal could be very useful
asm(R"(
.globl func
    .type func, @function
    func:
    .cfi_startproc
    movl $7, %eax
    ret
    .cfi_endproc
)"); // UndCC_Valid

int main()
{
    int n = func();
    asm("leal (%0,%0,4),%0" // UndCC_Violation
        : "=r"(n)
        : "0"(n));
    std::cout << "7*5 = " << n << std::endl;
    

    asm("movq $60, %rax\n\t"    // UndCC_Violation
        "movq $2,  %rdi\n\t"
        "syscall"); // UndCC_Valid, in-line comment
}