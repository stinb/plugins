// MISRA C++ 2008

void nullStmtComment1(int port)
{
    while ( ( port & 0x80 ) == 0 )
    {
        ; // wait for pin - UndCC_Valid
        /* wait for pin */ ; // UndCC_Violation, comment before ;
        ;// wait for pin – UndCC_Violation, no white-space char after ;
    }
}
