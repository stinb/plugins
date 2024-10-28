// Bad prefix and suffix
enum prefix_suffix {}; // UndCC_Violation(prefix_suffix_bad,caps_all,caps_upper,char_alpha,char_alpha_num,length,regexp)
enum prefi_suffix {}; // UndCC_Violation(prefix_suffix_bad,prefix_suffix_required,caps_all,caps_upper,char_alpha,char_alpha_num,length,regexp)
enum prefix_uffix {}; // UndCC_Violation(prefix_suffix_bad,prefix_suffix_required,caps_all,caps_upper,char_alpha,char_alpha_num,length,regexp)
enum missing_both {}; // UndCC_Violation(prefix_suffix_required,caps_all,caps_upper,char_alpha,char_alpha_num,length,regexp)
