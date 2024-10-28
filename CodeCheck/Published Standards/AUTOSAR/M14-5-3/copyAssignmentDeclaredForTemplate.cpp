class A // UndCC_Violation
{
public:
    // A & operator= ( A const & rhs ) // Example 1 - implicitly generated
    // {
    //     i = rhs.i;
    //     return *this;
    // }
    template <typename T>
    T & operator= ( T const & rhs ) // Example 2
    {
        if ( this != &rhs ) {
            delete i;
            i = new int;
            *i = *rhs.i;
        }
        return *this;
    }
private:
    int * i; // Member requires deep copy
};
void f ( A const & a1, A & a2 )
{
    a2 = a1; // Unexpectedly uses Example 1
}



class B // UndCC_Valid
{
public:
    B & operator= ( B const & rhs ) // Example 1
    {
        i = rhs.i;
        return *this;
    }
    template <typename T>
    T & operator= ( T const & rhs ) // Example 2
    {
        if ( this != &rhs ) {
            delete i;
            i = new int;
            *i = *rhs.i;
        }
        return *this;
    }
private:
    int * i; // Member requires deep copy
};
void f ( B const & b1, B & b2 )
{
    b2 = b1; // Expectedly uses Example 1
}



class C // UndCC_Valid
{
public:
    C & operator= ( C const & rhs ) // Example 1
    {
        i = rhs.i;
        return *this;
    }
    // template <typename T>
    // T & operator= ( T const & rhs ) // Example 2
    // {
    //     if ( this != &rhs ) {
    //         delete i;
    //         i = new int;
    //         *i = *rhs.i;
    //     }
    //     return *this;
    // }
private:
    int * i; // Member requires deep copy
};
void f ( C const & c1, C & c2 )
{
    c2 = c1; // Expectedly uses Example 1
}



class D // UndCC_Valid
{
public:
    // D & operator= ( D const & rhs ) // Example 1
    // {
    //     i = rhs.i;
    //     return *this;
    // }
    // template <typename T>
    // T & operator= ( T const & rhs ) // Example 2
    // {
    //     if ( this != &rhs ) {
    //         delete i;
    //         i = new int;
    //         *i = *rhs.i;
    //     }
    //     return *this;
    // }
private:
    int * i; // Member requires deep copy
};
void f ( D const & d1, D & d2 )
{
    d2 = d1; // Expectedly uses Example 1
}
