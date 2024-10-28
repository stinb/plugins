class Base
{
public:
    int x;

protected:
    int y;

private:
    int z;
};

class PublicDerived : public Base
{
    // x is public
    // y is protected
    // z is not accessible from PublicDerived
};

class ProtectedDerived : protected Base // UndCC_Violation
{
    // x is protected
    // y is protected
    // z is not accessible from ProtectedDerived
};

class PrivateDerived : private Base // UndCC_Violation
{
    // x is private
    // y is private
    // z is not accessible from PrivateDerived
};