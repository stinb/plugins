void DoSomething ( void )
{
}
void Delay ( void )
{
	asm ( "NOP" );    // UNDCC_Valid
}
void fn ( void )
{
	DoSomething ( );
	Delay ( );        // UNDCC_Valid - Assembler is encapsulated
	DoSomething ( );
	asm ( "NOP" );    // UNDCC_Violation
	DoSomething ( );
}
