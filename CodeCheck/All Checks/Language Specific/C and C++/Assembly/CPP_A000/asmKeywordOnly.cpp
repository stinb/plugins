void Delay_a ( void )
{
    asm ( "NOP" );   // UNDCC_Valid
}
void Delay_b ( void )
{
    __asm__ ( "NOP" ); // UndCC_Violation

    #pragma asm // UndCC_Violation
    "NOP";
    #pragma endasm
}
