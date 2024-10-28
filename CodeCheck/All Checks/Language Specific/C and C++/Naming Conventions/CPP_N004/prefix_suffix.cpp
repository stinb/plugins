// Bad prefix and suffix
void prefix_suffix() {} // UndCC_Violation(prefix_suffix_bad,caps_all,caps_upper,char_alpha,char_alpha_num,length,regexp)
void prefi_suffix() {} // UndCC_Violation(prefix_suffix_bad,prefix_suffix_required,caps_all,caps_upper,char_alpha,char_alpha_num,length,regexp)
void prefix_uffix() {} // UndCC_Violation(prefix_suffix_bad,prefix_suffix_required,caps_all,caps_upper,char_alpha,char_alpha_num,length,regexp)
void missing_both() {} // UndCC_Violation(prefix_suffix_required,caps_all,caps_upper,char_alpha,char_alpha_num,length,regexp)
