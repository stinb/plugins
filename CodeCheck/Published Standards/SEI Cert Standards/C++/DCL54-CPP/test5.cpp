// Custom - placement new and placement delete

#include <new>

struct NewPlacement {
  void *operator new(std::size_t count, void *ptr) // UndCC_Violation
  {
    return ptr;
  }
};

struct NewPlacementNoExceptFalse {
  void *operator new(std::size_t count, void *ptr) noexcept(false) // UndCC_Violation
  {
    return ptr;
  }
};

struct NewPlacementNoExcept {
  void *operator new(std::size_t count, void *ptr) noexcept // UndCC_Valid
  {
    return ptr;
  }
};

struct NewPlacementNoExceptTrue {
  void *operator new(std::size_t count, void *ptr) noexcept(true) // UndCC_Valid
  {
    return ptr;
  }
};

struct CtorNewPlacement {
  CtorNewPlacement()
  {

  }

  void *operator new(std::size_t count, void *ptr) noexcept // UndCC_Violation
  {
    return ptr;
  }
};

struct CtorNoExceptNewPlacement {
  CtorNoExceptNewPlacement() noexcept
  {

  }

  void *operator new(std::size_t count, void *ptr) // UndCC_Violation
  {
    return ptr;
  }
};

struct CtorsNewPlacement {
  CtorsNewPlacement() noexcept
  {

  }

  CtorsNewPlacement(int)
  {

  }

  void *operator new(std::size_t count, void *ptr) noexcept // UndCC_Violation
  {
    return ptr;
  }
};

struct CtorNoExceptNewPlacementNoExcept {
  CtorNoExceptNewPlacementNoExcept() noexcept
  {

  }

  void *operator new(std::size_t count, void *ptr) noexcept // UndCC_Valid
  {
    return ptr;
  }
};
