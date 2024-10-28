// Kind and consecutive capitals
class Kinds {
  public:
    enum publicConsecutivECapitals {};    // UndCC_Violation(kind_all,kind_public,caps_all,caps_no,caps_upper,length,prefix_suffix_required,regexp)
    enum publicNoConsecutiveCapitals {}; // UndCC_Violation(caps_all,caps_no,caps_upper,length,prefix_suffix_required,regexp)
  protected:
    enum protectedConsecutivECapitals {}; // UndCC_Violation(kind_all,caps_all,caps_no,caps_upper,kind_protected,length,prefix_suffix_required,regexp)
    enum protectedNoConsecutiveCapitals {}; // UndCC_Violation(caps_all,caps_no,caps_upper,length,prefix_suffix_required,regexp)
  private:
    enum privateConsecutivECapitals {};   // UndCC_Violation(kind_all,kind_private,caps_all,caps_no,caps_upper,length,prefix_suffix_required,regexp)
    enum privateNoConsecutiveCapitals {}; // UndCC_Violation(caps_all,caps_no,caps_upper,length,prefix_suffix_required,regexp)
};
