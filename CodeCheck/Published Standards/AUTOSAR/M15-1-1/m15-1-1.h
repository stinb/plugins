// construction of E2 causes an exception to be thrown
class E2
{
public:
    E2()
    {
        throw 10;
    }
};