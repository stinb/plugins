void Delay_a ( void )
{
    asm ( "NOP" );   // UNDCC_Valid
}
void Delay_b ( void )
{
    #pragma asm // UndCC_Violation
    "NOP";
    #pragma endasm
}
