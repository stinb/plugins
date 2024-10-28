// Custom

enum class Visibility {
  Private,
  Protected,
  Public,
};

class D {
public:
  int get_int(Visibility visibility)
  {
    switch (visibility) {
      case Visibility::Private:
        return private_int;
      case Visibility::Protected:
        return protected_int;
      case Visibility::Public:
        return public_int;
    }
  }

  int *get_int_ptr(Visibility visibility)
  {
    switch (visibility) {
      case Visibility::Private:
        return private_int_ptr; // UndCC_Violation
      case Visibility::Protected:
        return protected_int_ptr; // UndCC_Violation
      case Visibility::Public:
        return public_int_ptr;
    }
  }

public:
  int public_int;
  int *public_int_ptr;

protected:
  int protected_int;
  int *protected_int_ptr;

private:
  int private_int;
  int *private_int_ptr;
};
