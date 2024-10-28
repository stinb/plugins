// Custom

#include <cstring>

// Non-trivial class (non-trivial move assignment operator)
class C {
public:
  C &operator=(C &&other)
  {
    std::strcpy(mString, other.getString());
    return *this;
  }

  char *getString() const;

  C *getParent() const;

private:
  char *mString;
};

void object(C &a, C &b)
{
  char aStr = *(a.getString());
  char bStr = *(b.getString());

  std::memset(&a, 0, sizeof(C));   // UndCC_Violation

  std::memcpy(&a, &b, sizeof(C));  // UndCC_Violation
  std::memmove(&a, &b, sizeof(C)); // UndCC_Violation
  std::strcpy(&aStr, &bStr);

  std::memcmp(&a, &b, sizeof(C));       // UndCC_Violation  
  std::strcmp(&aStr, &bStr);
}

void objectPtr(C *a, C *b)
{
  char *aStr = a->getString();
  char *bStr = b->getString();

  std::memset(a, 0, sizeof(C));  // UndCC_Violation

  std::memcpy(a, b, sizeof(C));  // UndCC_Violation
  std::memmove(a, b, sizeof(C)); // UndCC_Violation
  std::strcpy(aStr, bStr);

  std::memcmp(a, b, sizeof(C)); // UndCC_Violation
  std::strcmp(aStr, bStr);
}

void objectDot(C &a, C &b)
{
  std::memset(a.getParent(), 0, sizeof(C));              // UndCC_Violation

  std::memcpy(a.getParent(), b.getParent(), sizeof(C));  // UndCC_Violation
  std::memmove(a.getParent(), b.getParent(), sizeof(C)); // UndCC_Violation
  std::strcpy(a.getString(), b.getString());

  std::memcmp(a.getParent(), b.getParent(), sizeof(C)); 
  std::strcmp(a.getString(), b.getString());
}

void objectPtrArrow(C *a, C *b)
{
  std::memset(a->getParent(), 0, sizeof(C));               // UndCC_Violation

  std::memcpy(a->getParent(), b->getParent(), sizeof(C));  // UndCC_Violation
  std::memmove(a->getParent(), b->getParent(), sizeof(C)); // UndCC_Violation
  std::strcpy(a->getString(), b->getString());

  std::memcmp(a->getParent(), b->getParent(), sizeof(C));      
  std::strcmp(a->getString(), b->getString());
}
