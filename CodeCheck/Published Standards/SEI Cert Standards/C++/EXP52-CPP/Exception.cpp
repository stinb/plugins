void small(int x);
void large(long long x);

#define m(x)                                     \
    do                                           \
    {                                            \
        if (sizeof(x) == sizeof(int))            \
        {                                        \
            small(x);                            \
        }                                        \
        else if (sizeof(x) == sizeof(long long)) \
        {                                        \
            large(x);                            \
        }                                        \
    } while (0)

void f()
{
    int i = 0;
    m(++i);
}