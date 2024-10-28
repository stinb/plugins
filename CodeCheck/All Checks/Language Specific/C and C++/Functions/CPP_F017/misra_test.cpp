template <typename T>
void f1(T); // Overload # 1A
template <>
void f1<char *>(char *); // UndCC_Violation - explicit specialization of overload # 1A
template <typename T>
void f1(T *); // Overload # 1B

template <typename T>
void f2(T); // Overload # 2A
template <typename T>
void f2(T *);    // Overload # 2B
void f2(char *); // Overload # 2C - rule does not apply

template <typename T>
void f3(T);
template <>
void f3<char *>(char *); // UndCC_Violation(1) - explicit specialization of f3

void b(char *s)
{
    f1(s); // Calls overload # 1B, with T = char
    f2(s); // Calls overload # 2C
}
