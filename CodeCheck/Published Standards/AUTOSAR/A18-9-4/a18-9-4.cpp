#include <cstdint>
#include <iostream>
#include <utility>

template <typename T1, typename T2>
void F1(T1&& t1, T2&& t2) {
    // ...
}

template <typename T1, typename T2>
void F2(T1&& t1, T2&& t2)
{
    F1(std::forward<T1>(t1), std::forward<T2>(t2));
    ++t2;  // UNDCC_Violation
};

int main() {
    F2(0, 0); // Call F2 to avoid the compiler option "Delayed Template Parsing"
}
