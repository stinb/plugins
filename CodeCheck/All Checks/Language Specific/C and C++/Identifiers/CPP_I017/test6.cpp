// Custom

enum Enum0 {
  EnumValue = 0,
};

enum class EnumClass0 {
  EnumClassValue = 0,
};

enum struct EnumStruct0 {
  EnumStructValue = 0,
};

int f()
{
  enum Enum1 {
    EnumValue = 1, // UndCC_Violation
  };

  enum class EnumClass1 {
    EnumClassValue = 1, // UndCC_Valid - cannot access without ::
  };

  enum struct EnumStruct1 {
    EnumStructValue = 1, // UndCC_Valid - cannot access without ::
  };
}
