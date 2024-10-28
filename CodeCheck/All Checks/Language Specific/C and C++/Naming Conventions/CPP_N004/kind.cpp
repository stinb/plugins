// Kind and consecutive capitals
class Kinds {
  public:
    void publicConsecutivECapitals() {}    // UndCC_Violation(kind_all,kind_public,caps_all,caps_no,caps_upper,length,prefix_suffix_required,regexp) 
    void publicNoConsecutiveCapitals() {} // UndCC_Violation(caps_all,caps_no,caps_upper,length,prefix_suffix_required,regexp)
  protected:
    void protectedConsecutivECapitals() {} // UndCC_Violation(kind_all,caps_all,caps_no,caps_upper,kind_protected,length,prefix_suffix_required,regexp) 
    void protectedNoConsecutiveCapitals() {} // UndCC_Violation(caps_all,caps_no,caps_upper,length,prefix_suffix_required,regexp)
  private:
    void privateConsecutivECapitals() {}   // UndCC_Violation(kind_all,kind_private,caps_all,caps_no,caps_upper,length,prefix_suffix_required,regexp) 
    void privateNoConsecutiveCapitals() {} // UndCC_Violation(caps_all,caps_no,caps_upper,length,prefix_suffix_required,regexp)
};
