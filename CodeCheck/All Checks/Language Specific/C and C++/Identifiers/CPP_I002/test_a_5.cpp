// Custom

// Template parameters have no linkage
template < int a_N > // UndCC_Valid
int a_first ()
{
    return a_N;
}

template < int a_N > // UndCC_Valid
int a_second ()
{
    return a_N + 1;
}

int a_use ()
{
    return a_first< 1 >() + a_second< 2 >();
}
