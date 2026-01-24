class B
{
public:
    virtual ~B() = default;
};

class D : public virtual B
{
};

int main()
{
    D d;
    B *pB = &d;
    D *pD1 = reinterpret_cast<D *>(pB); // UndCC_Violation
    D *pD2 = dynamic_cast<D *>(pB);     // UndCC_Valid - pD2 may be null
    D &D3 = dynamic_cast<D &>(*pB);     // UndCC_Valid - may throw an exception
}