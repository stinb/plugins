// Kind and consecutive capitals
class Kinds {
  public:
    int publicConsecutivECapitals;    // UndCC_Violation(kind_all,kind_public,caps_all,caps_no,caps_upper,length,prefix_suffix_required,regexp)
    int publicNoConsecutiveCapitals;  // UndCC_Violation(caps_all,caps_no,caps_upper,length,prefix_suffix_required,regexp)
  protected:
    int protectedConsecutivECapitals; // UndCC_Violation(kind_all,kind_protected,caps_all,caps_no,caps_upper,length,prefix_suffix_required,regexp)
    int protectedNoConsecutiveCapitals; // UndCC_Violation(caps_all,caps_no,caps_upper,length,prefix_suffix_required,regexp)
  private:
    int privateConsecutivECapitals;   // UndCC_Violation(kind_all,kind_private,caps_all,caps_no,caps_upper,length,prefix_suffix_required,regexp)
    int privateNoConsecutiveCapitals; // UndCC_Violation(caps_all,caps_no,caps_upper,length,prefix_suffix_required,regexp)
};
