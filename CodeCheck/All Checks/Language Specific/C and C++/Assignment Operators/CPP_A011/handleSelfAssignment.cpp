// $Id: A12-8-5.cpp 271773 2017-03-23 13:16:53Z piotr.tanski $

struct A
{
    int number;
    int *ptr;
    // Implementation
};

class B
{
public:
    // ...
    B &operator=(B const &oth) // UndCC_Violation
    {
        i = oth.i;
        delete aPtr;

        try
        {
            aPtr = new A(*oth.aPtr); // If this is the self-copy case, then
            // the oth.a_ptr is already deleted
        }
        catch (...)
        {
            aPtr = nullptr;
        }

        return *this;
    }

private:
    short i = 0;
    A *aPtr = nullptr;
};

class C
{
public:
    C &operator=(C const &oth) // UndCC_Valid
    {
        if (this != &oth)
        {
            A *tmpPtr = new A(*oth.aPtr);

            i = oth.i;
            delete aPtr;
            aPtr = tmpPtr;
        }
        return *this;
    }
    C &operator=(C &&oth) // UndCC_Valid
    {
        if (this != &oth)
        {
            A *tmpPtr = new A(*oth.aPtr);

            i = oth.i;
            delete aPtr;
            aPtr = tmpPtr;
        }
        return *this;
    }

private:
    short i = 0;
    A *aPtr = nullptr;
};
