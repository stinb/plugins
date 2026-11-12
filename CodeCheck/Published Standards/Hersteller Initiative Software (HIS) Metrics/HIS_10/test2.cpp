class Widget {
public:
    Widget(int a)  // UndCC_Violation(default)
    {
        if (a)
            return;
        x = a;
    }

    ~Widget()  // UndCC_Violation(default)
    {
        if (x)
            return;
        x = 0;
    }

    int get()  // UndCC_Violation
    {
        if (x)
            return 1;
        return x;
    }

    int x;
};
