// Custom

// Whitespace & comments
#if 0
  #define zero 0
  # define zero 0
  # /* Comment */ define zero 0
  #define1 zero 0                // UndCC_Violation
  # define1 zero 0               // UndCC_Violation
  # /* Comment */ define1 zero 0 // UndCC_Violation
#endif

// Replacing text macros
#define OUTER(...) WORD #__VA_ARGS__

// All standard processor directives which follow #
#if 0
  #define
  #if
  #elif
  #elifdef
  #elifndef
  #else
  #endif
  #error
  #ifdef
  #endif
  #ifndef
  #endif
  #include
  #line
  #pragma
  #undef
  #warning
#endif

// Nothing
# // UndCC_Violation
