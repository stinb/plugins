// SEI CERT C++

template <typename Ty>
class has_foo_function
{
    typedef char yes[1];
    typedef char no[2];

    template <typename Inner>
    static yes &test(Inner *I, decltype(I->foo()) * = nullptr); // Function is never defined.

    template <typename>
    static no &test(...); // Function is never defined.

public:
    static const bool value = sizeof(test<Ty>(nullptr)) == sizeof(yes);
};
