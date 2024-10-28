#ifdef DEFINE_GLOBALS
  int sample_var;
#else
  extern int sample_var;
#endif
  
#ifdef DEFINE_GLOBALS
  #undef DEFINE_GLOBALS
#endif

int unguarded_var; // flag violation, global declared outside of include guards // UndCC_Violation

#ifdef DEFINE_GLOBALS  // flag violation, DEFINE_GLOBALS used after undef'd // UndCC_Violation
  int sample_var_2;
#else
  extern int sample_var_2;
#endif

#ifdef DEFINE_GLOBALS_2  // flag this macro as it is guarding a global but never undef'd // UndCC_Violation
  int sample_var_3;
#else
  extern int sample_var_3;
#endif
