// Custom

// Files
#include <signal.h> // UndCC_Violation
#include <stdlib.h> // UndCC_Valid

// Functions
void *raise_ptr  = raise;  // UndCC_Violation
void *signal_ptr = signal; // UndCC_Violation
void *abort_ptr  = abort;  // UndCC_Valid

// Macros
int a = SIG_DFL;      // UndCC_Violation
int b = SIG_ERR;      // UndCC_Violation
int c = SIG_IGN;      // UndCC_Violation
int d = SIGABRT;      // UndCC_Violation
int e = SIGFPE;       // UndCC_Violation
int f = SIGILL;       // UndCC_Violation
int g = SIGINT;       // UndCC_Violation
int h = SIGSEGV;      // UndCC_Violation
int i = SIGTERM;      // UndCC_Violation
int j = EXIT_SUCCESS; // UndCC_Valid

// Types
typedef sig_atomic_t SignalAtom; // UndCC_Violation
typedef int Other;               // UndCC_Valid
