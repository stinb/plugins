// Length of 2-3
enum a {};    // UndCC_Violation(length,caps_all,caps_upper,prefix_suffix_required,regexp)
enum aa {};   // UndCC_Violation(caps_all,caps_upper,prefix_suffix_required,regexp)
enum aaa {};  // UndCC_violation(caps_all,caps_upper,prefix_suffix_required,regexp)
enum aaaa {}; // UndCC_Violation(length,caps_all,caps_upper,prefix_suffix_required,regexp)
