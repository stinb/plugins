struct S
{
    unsigned char buffType;
    int size;

    friend bool operator==(const S &lhs, const S &rhs)
    {
        return lhs.buffType == rhs.buffType &&
               lhs.size == rhs.size;
    }
};

void f(const S &s1, const S &s2)
{
    if (s1 == s2)
    {
        // ...
    }
}