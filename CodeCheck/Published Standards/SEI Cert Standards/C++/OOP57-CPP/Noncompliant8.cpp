// Custom

#include <cstring>

// Non-trivial class (non-trivial destructor)
class C {
public:
  ~C()
  {
    delete mChild;
  }

  char *getString() const;

  C *getParent() const;
  C *getChild() const;

private:
  C *mChild;
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
