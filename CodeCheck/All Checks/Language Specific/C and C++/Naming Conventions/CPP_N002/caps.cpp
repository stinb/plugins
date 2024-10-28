// Capitalization
enum UpperCamelCase {}; // UndCC_Violation(caps_all,caps_lower,caps_no,length,prefix_suffix_required,regexp)
enum lowerCamelCase {}; // UndCC_Violation(caps_all,caps_no,caps_upper,length,prefix_suffix_required,regexp)
enum ALL_CAPS {};       // UndCC_Violation(caps_lower,caps_no,char_alpha,char_alpha_num,kind_all,length,prefix_suffix_required,regexp)
enum no_caps {};        // UndCC_Violation(caps_all,caps_upper,char_alpha,char_alpha_num,length,prefix_suffix_required,regexp)
