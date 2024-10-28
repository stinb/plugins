auto g(int val)
{
    auto outer = [val]
    {
        int i = val;
        auto inner = [i]
        {
            return i + 30;
        };
        return inner;
    };
    return outer();
}

void f()
{
    auto fn = g(12);
    int j = fn();
}