// Length of 2-3
void a() {}    // UndCC_Violation(length,caps_all,caps_upper,prefix_suffix_required,regexp) 
void aa() {}   // UndCC_Violation(caps_all,caps_upper,prefix_suffix_required,regexp)
void aaa() {}  // UndCC_violation(caps_all,caps_upper,prefix_suffix_required,regexp)
void aaaa() {} // UndCC_Violation(length,caps_all,caps_upper,prefix_suffix_required,regexp) 
