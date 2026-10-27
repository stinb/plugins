template <class T> struct Box {   // UndCC_Valid
  Box();
  void used_mem();
  void unused_mem();
  static int s;
  int field;                      // UndCC_Valid
};
