#include <cstdint>
#include <string>

// Global variables for testing
uint32_t u32, u32a;
uint16_t u16, u16a;
uint8_t u8, u8a, u8b;
int32_t s32;
int16_t s16;
int8_t s8;
float flt1, flt2, flt3, flt4, flt5;
float f32;
int64_t s64;
bool b;
uint8_t array[10];

constexpr int32_t fn(int32_t i) { return i * i; }

// Tests from the introductory examples
void test_intro_examples()
{
    b ? u8a : u8b;                 // UndCC_Valid - no conversion
    b ? u8a : u16a;                // UndCC_Violation - ternary operator - Non-compliant - u8a -> signed int and u16a -> signed int

    array[u8];                     // UndCC_Valid - rule does not apply - no conversion of u8
    u8++;                          // UndCC_Valid - rule does not apply

    f32 += u32;                    // UndCC_Violation - compound assignment (partial implementation) - Non-compliant - u32 -> floating
    f32 += 1;                      // UndCC_Valid - Compliant by exception #2 (not a false negative, correctly not flagged)
    f32 += 0x100'0001;             // UndCC_Valid - Compliant by exception #2 - precision lost (not a false negative, correctly not flagged)

    ~u8;                           // UndCC_Violation - unary operators - Non-compliant - u8 -> signed int
    ~u32;                          // UndCC_Valid - Compliant
    -u8;                           // UndCC_Violation - unary operators - Non-compliant - u8 -> signed int
    u8 << 2;                       // UndCC_Violation - shift operators - Non-compliant - u8 -> signed int

    u8 + fn(10);                   // UndCC_Valid - Compliant by exception #1 (constexpr function exception - not a false negative, correctly not flagged)
    f32 + fn(10);                  // UndCC_Valid - Compliant by exception #2 (constexpr function exception - not a false negative, correctly not flagged)
}

// Basic assignment tests
void test_basic_assignments()
{
    u32 = 1;                  // UndCC_Valid - same type
    s32 = 4u * 2u;            // UndCC_Valid - constant expression fits
    u8 = 3u;                  // UndCC_Valid - constant fits
    u8 = 300u;                // UndCC_FalseNeg - constant expressions skipped - value does not fit

    u8 = s8;                  // UndCC_Violation - different signedness
    u8 = u8 + u8;             // UndCC_Violation - narrowing from int
    flt1 = s32;               // UndCC_Violation - different type category
    flt2 = 0.0;               // UndCC_FalseNeg - constant expressions skipped - different sizes (double to float)
    flt3 = 0.0f;              // UndCC_Valid - same type
    flt4 = 1;                 // UndCC_Valid - constant fits
    flt5 = 9999999999;        // UndCC_Valid - constant fits (precision loss allowed)
}

// Bit-field tests
struct S {
    uint32_t b : 2;           // Bit-field is considered to be uint8_t
};

void test_bitfields()
{
    S s;
    s.b = 2;                      // UndCC_Valid - Compliant (bit-field handling not implemented but this is compliant)
    s.b = 32u;                    // UndCC_FalseNeg - bit-field handling not implemented - Non-compliant - value does not fit
    s.b = u8;                     // UndCC_Valid - Compliant - same width, but may truncate (bit-field handling not implemented but this is compliant)
    s.b = u16;                    // UndCC_FalseNeg - bit-field handling not implemented - Non-compliant - narrowing
}

// Overload tests
void sb1(uint32_t);
void sb1(uint8_t);
void sb2(uint8_t);

void test_overload_bitfield()
{
    S s;
    sb1(s.b);                 // UndCC_FalseNeg - overload resolution checking not implemented - Non-compliant - s.b considered to be uint8_t, but sb1(uint32_t) is called
    sb2(s.b);                 // UndCC_Valid - Compliant (overload resolution checking not implemented but this is compliant)
}

// Enum tests
void test_enum()
{
    enum Colour : uint16_t { red, green, blue } c;
    u8 = red;                 // UndCC_Valid - value can be represented
    u32 = red;                // UndCC_Valid - value can be represented
    u8 = c;                   // UndCC_FalseNeg - enum variables skipped to avoid false positives - Non-compliant - different sizes (narrowing)
    u32 = c;                  // UndCC_Valid - Compliant - widening of id-expression (enum variables skipped but this is compliant)

    enum States { enabled, disabled };
    u8 = enabled;             // UndCC_FalseNeg - non-numeric enum detection incomplete - Rule does not apply - source type not numeric
}

// Narrowing and widening tests
void test_narrowing_and_widening()
{
    unsigned long ul;
    unsigned int ui = ul;     // UndCC_FalsePos(Lin,Mac) - if sizes are equal (implementation defined)

    int16_t val1 = s8;        // UndCC_Valid - widening of id-expression
    int16_t val2{s8};         // UndCC_Valid - widening of id-expression
    int16_t val3(s8);         // UndCC_Valid - widening of id-expression
    int16_t val4{s8 + s8};    // UndCC_Violation - narrowing (s8 + s8 is int)
}

// Return statement tests
int test_return(int8_t s8)
{
    return s8;                // UndCC_Valid - widening of id-expression
}

int test_return_expr(int8_t s8)
{
    return s8 + s8;           // UndCC_Valid - s8 + s8 is already int
}

// Switch case tests
void test_switch(int8_t s8)
{
    switch(s8)
    {
        case 1:               // UndCC_Valid
        case 127:             // UndCC_Valid
        case 0xFFFF'FFFF'FFFF: // UndCC_Violation - switch case value - Non-compliant - value does not fit in int8_t
            break;
    }
}

// Function parameter tests
void f1(int64_t i);
void f2(int i);

void test_function_params()
{
    f1(s32 + s32);            // UndCC_Violation - function argument - Non-compliant - implicit widening conversion
    f2(s32 + s64);            // UndCC_Violation - function argument - Non-compliant - implicit narrowing conversion
    f2(s16 + s16);            // UndCC_Valid - result of addition is int
}

// Constructor tests
struct A {
    explicit A(int32_t i);
    explicit A(int64_t i);
};

void test_constructors()
{
    A a{s16};                     // UndCC_FalseNeg - constructor exception not implemented - Non-compliant
}

// Function pointer and overload-independent tests
void f3(long l);

void test_function_pointers()
{
    void (*fp)(long l) = &f3;
    f3(2);                        // UndCC_FalseNeg - constant expressions skipped - Non-compliant - implicit conversion from int to long
    fp(2);                        // UndCC_Valid - Compliant - calling through function pointer is overload-independent (not a false negative, correctly not flagged)
}

// Constructor exception tests
struct MyInt {
    explicit MyInt(int32_t);
    MyInt(int32_t, int32_t);
};

void f4(MyInt);

void test_constructor_exception(int16_t s)
{
    f4(MyInt{s});             // UndCC_Valid - Compliant by exception - no need to cast s (constructor widening exception not implemented but this is compliant)
    MyInt i{s};               // UndCC_Valid - Compliant by exception - no need to cast s (constructor widening exception not implemented but this is compliant)
}

// Ellipsis parameter tests
void log(char const *fmt, ...);

void test_ellipsis(uint8_t c)
{
    log("f( %c )", c);        // UndCC_Violation - Non-compliant - conversion of c from uint8_t to int
}

// Overload-independent parameter tests
struct AClass {
    void set(short value);
    void set(size_t index, int value);
    void set(size_t index, std::string value);
    void set(int index, double value) = delete;  // Not callable
    void g();
};

void test_overload_independent(AClass &a)
{
    a.set(42, "answer");      // UndCC_Valid - Compliant - size_t can represent 42, and it is assigned to an overload-independent parameter (not a false negative, correctly not flagged)
}

void AClass::g()
{
    set(42, "answer");        // UndCC_FalseNeg - constant expressions skipped - Non-compliant - even though this non-qualified call will only select an overload in the class
}

struct BClass {
    void set(int index, int value);
    void set(long index, std::string value);
};

void test_not_overload_independent(BClass &b)
{
    b.set(42, "answer");      // UndCC_FalseNeg - constant expressions skipped - Non-compliant - conversion from int to long not allowed as parameter is not overload-independent
}

// Aggregate initialization tests
struct C {
    int32_t x;
    int64_t y;
    int64_t z;
};

void test_aggregate_init()
{
    C c1{s16 + s16,               // UndCC_Valid - Compliant - s16 + s16 is of type int (aggregate initialization checking incomplete but this is compliant)
         s16 + s16,               // UndCC_Violation - Non-compliant - widening from int
         s16};                    // UndCC_Valid - Compliant - widening of id-expression (aggregate initialization checking incomplete but this is compliant)
}

// Template tests
template<typename T>
struct D {
    void set1(T index, int value);
    void set1(T index, std::string value);
    template<typename S1>
    void set2(S1 index, int value);
    template<typename S2>
    void set2(S2 index, std::string value);
};

void test_templates(D<size_t> &a)
{
    a.set1(42, "X");          // UndCC_Valid - Compliant - size_t is same type (template parameter checking not implemented but this is compliant)
    a.set2<size_t>(42, "X");  // UndCC_FalseNeg - constant expressions skipped - Non-compliant - 'S1' is never the same as the specialized type of 'S2' (size_t)
}
