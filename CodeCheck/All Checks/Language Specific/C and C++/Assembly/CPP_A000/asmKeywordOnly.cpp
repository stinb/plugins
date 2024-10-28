#DEFINE NOP \
    asm("   NOP"); // UndCC_Valid

void Delay ( void ) 
{ 
  _asm ( "NOP" );     // UndCC_Violation
}

__asm ( "NOP" );    // UndCC_Violation 

void fn ( void ) 
{ 
  DoSomething ( ); 
  Delay ( );        // Assembler is encapsulated 
  DoSomething ( ); 
  __asm__ ( "NOP" );    // UndCC_Violation 
  DoSomething ( ); 
  NOP;    // UndCC_Valid
  DoSomething();
}

void Delay_b ( void ) 
{ 
#pragma asm 
  "NOP"            
#pragma endasm  // Not Compliant
}
