#include <cstdint>
#include <utility>

// ============================================================
// Sub-requirement 1: Valid copyability category
// ============================================================

// Unmovable: !cc, !mc, !ca, !ma
class Unmovable { // UndCC_Valid
public:
  Unmovable() = default;
  ~Unmovable() = default;
  Unmovable(const Unmovable &) = delete;
  Unmovable(Unmovable &&) = delete;
  Unmovable &operator=(const Unmovable &) = delete;
  Unmovable &operator=(Unmovable &&) = delete;
};

// Move-only: !cc, mc, !ca, ma optional
class MoveOnly { // UndCC_Valid
public:
  MoveOnly() = default;
  MoveOnly(const MoveOnly &) = delete;
  MoveOnly(MoveOnly &&) = default;
  MoveOnly &operator=(const MoveOnly &) = delete;
  MoveOnly &operator=(MoveOnly &&) = default;
};

// Copy-enabled with both copy and move assignment
class CopyEnabled { // UndCC_Valid
public:
  CopyEnabled() = default;
  ~CopyEnabled() = default;
  CopyEnabled(const CopyEnabled &) = default;
  CopyEnabled(CopyEnabled &&) = default;
  CopyEnabled &operator=(const CopyEnabled &) = default;
  CopyEnabled &operator=(CopyEnabled &&) = default;
};

// Copy-enabled without assignment (ca and ma both absent)
class CopyEnabledNoAssign { // UndCC_Valid
public:
  CopyEnabledNoAssign() = default;
  ~CopyEnabledNoAssign() = default;
  CopyEnabledNoAssign(const CopyEnabledNoAssign &) = default;
  CopyEnabledNoAssign(CopyEnabledNoAssign &&) = default;
  CopyEnabledNoAssign &operator=(const CopyEnabledNoAssign &) = delete;
  CopyEnabledNoAssign &operator=(CopyEnabledNoAssign &&) = delete;
};

// Invalid: cc but no mc (copy without move)
class InvalidCopyNoMove { // UndCC_FalseNeg - invalid copyability category (sub-req 1 disabled)
public:
  InvalidCopyNoMove() = default;
  InvalidCopyNoMove(const InvalidCopyNoMove &) = default;
  InvalidCopyNoMove(InvalidCopyNoMove &&) = delete;
  InvalidCopyNoMove &operator=(const InvalidCopyNoMove &) = default;
  InvalidCopyNoMove &operator=(InvalidCopyNoMove &&) = delete;
};

// Invalid: cc, mc, ca but no ma (asymmetric assignment)
class InvalidAsymmetricAssign { // UndCC_FalseNeg - invalid copyability category (sub-req 1 disabled)
public:
  InvalidAsymmetricAssign() = default;
  InvalidAsymmetricAssign(const InvalidAsymmetricAssign &) = default;
  InvalidAsymmetricAssign(InvalidAsymmetricAssign &&) = default;
  InvalidAsymmetricAssign &operator=(const InvalidAsymmetricAssign &) = default;
  InvalidAsymmetricAssign &operator=(InvalidAsymmetricAssign &&) = delete;
};


// ============================================================
// Sub-requirement 2: Customized copy/move -> customized destructor
// ============================================================

// Customized copy ctor but defaulted destructor
class CustomCopyDefaultDtor {
public:
  CustomCopyDefaultDtor() = default;
  ~CustomCopyDefaultDtor() = default; // UndCC_Violation - dtor not customized
  CustomCopyDefaultDtor(const CustomCopyDefaultDtor &other) { (void)other; }
  CustomCopyDefaultDtor(CustomCopyDefaultDtor &&other) { (void)other; }
  CustomCopyDefaultDtor &operator=(const CustomCopyDefaultDtor &) = default;
  CustomCopyDefaultDtor &operator=(CustomCopyDefaultDtor &&) = default;
};

// Customized move assignment but no customized destructor
class CustomMoveAssignDefaultDtor {
public:
  CustomMoveAssignDefaultDtor() = default;
  ~CustomMoveAssignDefaultDtor() = default; // UndCC_Violation - dtor not customized
  CustomMoveAssignDefaultDtor(const CustomMoveAssignDefaultDtor &) = default;
  CustomMoveAssignDefaultDtor(CustomMoveAssignDefaultDtor &&) = default;
  CustomMoveAssignDefaultDtor &operator=(const CustomMoveAssignDefaultDtor &) = default;
  CustomMoveAssignDefaultDtor &operator=(CustomMoveAssignDefaultDtor &&other) {
    (void)other;
    return *this;
  }
};


// ============================================================
// Sub-requirement 3: Customized destructor must be non-empty
// ============================================================

// Empty destructor body
class EmptyDtor {
public:
  int *p = nullptr;
  EmptyDtor() = default;
  ~EmptyDtor() {} // UndCC_Violation - empty body
  EmptyDtor(const EmptyDtor &other) : p(other.p) {}
  EmptyDtor(EmptyDtor &&other) : p(other.p) { other.p = nullptr; }
  EmptyDtor &operator=(const EmptyDtor &other) { p = other.p; return *this; }
  EmptyDtor &operator=(EmptyDtor &&other) { p = other.p; other.p = nullptr; return *this; }
};

// Destructor with only null statements
class NullStmtDtor {
public:
  int *p = nullptr;
  NullStmtDtor() = default;
  ~NullStmtDtor() { ; } // UndCC_Violation - empty body (null statements only)
  NullStmtDtor(const NullStmtDtor &other) : p(other.p) {}
  NullStmtDtor(NullStmtDtor &&other) : p(other.p) { other.p = nullptr; }
  NullStmtDtor &operator=(const NullStmtDtor &other) { p = other.p; return *this; }
  NullStmtDtor &operator=(NullStmtDtor &&other) { p = other.p; other.p = nullptr; return *this; }
};

// Non-empty destructor - compliant
class NonEmptyDtor { // UndCC_Valid
public:
  int *p = nullptr;
  NonEmptyDtor() : p(new int(0)) {}
  ~NonEmptyDtor() { delete p; }
  NonEmptyDtor(const NonEmptyDtor &other) : p(new int(*other.p)) {}
  NonEmptyDtor(NonEmptyDtor &&other) : p(other.p) { other.p = nullptr; }
  NonEmptyDtor &operator=(const NonEmptyDtor &other) {
    if (this != &other) { delete p; p = new int(*other.p); }
    return *this;
  }
  NonEmptyDtor &operator=(NonEmptyDtor &&other) {
    if (this != &other) { delete p; p = other.p; other.p = nullptr; }
    return *this;
  }
};


// ============================================================
// Sub-requirement 4: Unique manager (move-only + customized dtor)
// ============================================================

// Move-only with customized dtor but defaulted move ctor
class MoveOnlyDefaultedMoveCtor {
public:
  int *p = nullptr;
  MoveOnlyDefaultedMoveCtor() = default;
  ~MoveOnlyDefaultedMoveCtor() { delete p; }
  MoveOnlyDefaultedMoveCtor(const MoveOnlyDefaultedMoveCtor &) = delete;
  MoveOnlyDefaultedMoveCtor(MoveOnlyDefaultedMoveCtor &&) = default; // UndCC_FalseNeg - needs customized move ctor (sub-req 4 disabled)
  MoveOnlyDefaultedMoveCtor &operator=(const MoveOnlyDefaultedMoveCtor &) = delete;
  MoveOnlyDefaultedMoveCtor &operator=(MoveOnlyDefaultedMoveCtor &&other) {
    if (this != &other) { delete p; p = other.p; other.p = nullptr; }
    return *this;
  }
};

// Compliant move-only unique manager
class UniqueManager { // UndCC_Valid
public:
  int *p = nullptr;
  UniqueManager() = default;
  ~UniqueManager() { delete p; }
  UniqueManager(const UniqueManager &) = delete;
  UniqueManager(UniqueManager &&other) : p(other.p) { other.p = nullptr; }
  UniqueManager &operator=(const UniqueManager &) = delete;
  UniqueManager &operator=(UniqueManager &&other) {
    if (this != &other) { delete p; p = other.p; other.p = nullptr; }
    return *this;
  }
};


// ============================================================
// Sub-requirement 5: General manager (copy-enabled + customized dtor)
// ============================================================

// Copy-enabled with customized dtor but defaulted copy ctor
class GeneralDefaultedCopyCtor {
public:
  int *p = nullptr;
  GeneralDefaultedCopyCtor() = default;
  ~GeneralDefaultedCopyCtor() { delete p; }
  GeneralDefaultedCopyCtor(const GeneralDefaultedCopyCtor &) = default; // UndCC_FalseNeg - needs customized copy ctor (sub-req 5 disabled)
  GeneralDefaultedCopyCtor(GeneralDefaultedCopyCtor &&other) : p(other.p) {
    other.p = nullptr;
  }
  GeneralDefaultedCopyCtor &operator=(const GeneralDefaultedCopyCtor &other) {
    if (this != &other) { delete p; p = new int(*other.p); }
    return *this;
  }
  GeneralDefaultedCopyCtor &operator=(GeneralDefaultedCopyCtor &&other) {
    if (this != &other) { delete p; p = other.p; other.p = nullptr; }
    return *this;
  }
};

// Compliant general manager
class GeneralManager { // UndCC_Valid
public:
  int *p = nullptr;
  GeneralManager() : p(new int(0)) {}
  ~GeneralManager() { delete p; }
  GeneralManager(const GeneralManager &other) : p(new int(*other.p)) {}
  GeneralManager(GeneralManager &&other) : p(other.p) { other.p = nullptr; }
  GeneralManager &operator=(const GeneralManager &other) {
    if (this != &other) { delete p; p = new int(*other.p); }
    return *this;
  }
  GeneralManager &operator=(GeneralManager &&other) {
    if (this != &other) { delete p; p = other.p; other.p = nullptr; }
    return *this;
  }
};


// ============================================================
// Sub-requirement 6: Base class requirements
// ============================================================

// Compliant: unmovable base with public virtual destructor
class UnmovableBase { // UndCC_Valid
public:
  UnmovableBase() = default;
  virtual ~UnmovableBase() = default;
  UnmovableBase(const UnmovableBase &) = delete;
  UnmovableBase(UnmovableBase &&) = delete;
  UnmovableBase &operator=(const UnmovableBase &) = delete;
  UnmovableBase &operator=(UnmovableBase &&) = delete;
};

class DerivedFromUnmovable : public UnmovableBase { // UndCC_Valid
public:
  DerivedFromUnmovable() = default;
  ~DerivedFromUnmovable() override = default;
  DerivedFromUnmovable(const DerivedFromUnmovable &) = delete;
  DerivedFromUnmovable(DerivedFromUnmovable &&) = delete;
  DerivedFromUnmovable &operator=(const DerivedFromUnmovable &) = delete;
  DerivedFromUnmovable &operator=(DerivedFromUnmovable &&) = delete;
};

// Compliant: protected non-virtual destructor
class ProtectedDtorBase { // UndCC_Valid
public:
  ProtectedDtorBase() = default;
  ProtectedDtorBase(const ProtectedDtorBase &) = delete;
  ProtectedDtorBase(ProtectedDtorBase &&) = delete;
  ProtectedDtorBase &operator=(const ProtectedDtorBase &) = delete;
  ProtectedDtorBase &operator=(ProtectedDtorBase &&) = delete;
protected:
  ~ProtectedDtorBase() = default;
};

class DerivedFromProtected : public ProtectedDtorBase { // UndCC_Valid
public:
  DerivedFromProtected() = default;
  ~DerivedFromProtected() = default;
  DerivedFromProtected(const DerivedFromProtected &) = delete;
  DerivedFromProtected(DerivedFromProtected &&) = delete;
  DerivedFromProtected &operator=(const DerivedFromProtected &) = delete;
  DerivedFromProtected &operator=(DerivedFromProtected &&) = delete;
};

// Non-compliant: public non-virtual destructor on non-unmovable base
class BadBase { // UndCC_Valid - the violation is on the derived class
  int data = 0; // private member makes this non-aggregate
public:
  BadBase() = default;
  ~BadBase() = default;
  BadBase(const BadBase &) = default;
  BadBase(BadBase &&) = default;
  BadBase &operator=(const BadBase &) = default;
  BadBase &operator=(BadBase &&) = default;
};

class DerivedFromBadBase : public BadBase { // UndCC_FalseNeg - base class doesn't meet requirements (sub-req 6 disabled)
public:
  DerivedFromBadBase() = default;
  ~DerivedFromBadBase() = default;
  DerivedFromBadBase(const DerivedFromBadBase &) = default;
  DerivedFromBadBase(DerivedFromBadBase &&) = default;
  DerivedFromBadBase &operator=(const DerivedFromBadBase &) = default;
  DerivedFromBadBase &operator=(DerivedFromBadBase &&) = default;
};

// Exception: aggregate base class
struct AggregateBase { // UndCC_Valid - aggregate
  int x;
  int y;
};

class DerivedFromAggregate : public AggregateBase { // UndCC_Valid - aggregate exception
public:
  DerivedFromAggregate() = default;
  ~DerivedFromAggregate() = default;
  DerivedFromAggregate(const DerivedFromAggregate &) = delete;
  DerivedFromAggregate(DerivedFromAggregate &&) = delete;
  DerivedFromAggregate &operator=(const DerivedFromAggregate &) = delete;
  DerivedFromAggregate &operator=(DerivedFromAggregate &&) = delete;
};


// ============================================================
// Class with all defaulted - compliant
// ============================================================

class AllDefaulted { // UndCC_Valid
public:
  AllDefaulted() = default;
  ~AllDefaulted() = default;
  AllDefaulted(const AllDefaulted &) = default;
  AllDefaulted(AllDefaulted &&) = default;
  AllDefaulted &operator=(const AllDefaulted &) = default;
  AllDefaulted &operator=(AllDefaulted &&) = default;
};

// No user-declared SMFs - implicitly has all five - compliant
class ImplicitAll { // UndCC_Valid
  int x;
};


// ============================================================
// Additional edge cases
// ============================================================

// General manager with customized copy but no move ops declared (compliant)
class GeneralManagerNoMove { // UndCC_Valid
public:
  int *p = nullptr;
  GeneralManagerNoMove() : p(new int(0)) {}
  ~GeneralManagerNoMove() { delete p; }
  GeneralManagerNoMove(const GeneralManagerNoMove &other) : p(new int(*other.p)) {}
  GeneralManagerNoMove &operator=(const GeneralManagerNoMove &other) {
    if (this != &other) { delete p; p = new int(*other.p); }
    return *this;
  }
};

// Copy customized, move defaulted (violation: move must be customized or not declared)
class CopyCustomMoveDefault {
public:
  int *p = nullptr;
  CopyCustomMoveDefault() : p(new int(0)) {}
  ~CopyCustomMoveDefault() { delete p; }
  CopyCustomMoveDefault(const CopyCustomMoveDefault &other) : p(new int(*other.p)) {}
  CopyCustomMoveDefault(CopyCustomMoveDefault &&) = default; // UndCC_FalseNeg - move must be customized or not declared (sub-req 5 disabled)
  CopyCustomMoveDefault &operator=(const CopyCustomMoveDefault &other) {
    if (this != &other) { delete p; p = new int(*other.p); }
    return *this;
  }
  CopyCustomMoveDefault &operator=(CopyCustomMoveDefault &&) = default; // UndCC_FalseNeg - move ops must both be customized or both not declared (sub-req 5 disabled)
};

// Move-only base used as public base (violation: only unmovable allowed)
class MoveOnlyBase {
public:
  MoveOnlyBase() = default;
  virtual ~MoveOnlyBase() = default;
  MoveOnlyBase(const MoveOnlyBase &) = delete;
  MoveOnlyBase(MoveOnlyBase &&) = default;
  MoveOnlyBase &operator=(const MoveOnlyBase &) = delete;
  MoveOnlyBase &operator=(MoveOnlyBase &&) = default;
};

class DerivedFromMoveOnly : public MoveOnlyBase { // UndCC_FalseNeg - base is not unmovable (sub-req 6 disabled)
public:
  DerivedFromMoveOnly() = default;
  DerivedFromMoveOnly(const DerivedFromMoveOnly &) = delete;
  DerivedFromMoveOnly(DerivedFromMoveOnly &&) = default;
  DerivedFromMoveOnly &operator=(const DerivedFromMoveOnly &) = delete;
  DerivedFromMoveOnly &operator=(DerivedFromMoveOnly &&) = default;
};
