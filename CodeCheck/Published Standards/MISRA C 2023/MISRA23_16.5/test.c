void foo()
{

    int x;

    switch (x)
    {
    default: /* UndCC_Valid - default is the first label */
    case 0:
        ++x;
        break;
    case 1:
    case 2:
        break;
    }

    switch (x)
    {
    case 0:
        ++x;
        break;
    default: /* UndCC_Violation - default is mixed with the case labels */
        x = 0;
        break;
    case 1:
    case 2:
        break;
    }

    switch (x)
    {
    case 0:
        ++x;
        break;
    case 1:
    case 2:
        break;
    default: /* UndCC_Valid - default is the final label */
        x = 0;
        break;
    }
}
