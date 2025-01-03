// AUTOSAR

#include <cstdint>
#include <memory>
#include <stdexcept>
#include <vector>

std::int32_t Fn1()
{
std::int32_t errorCode{0};

std::int32_t* ptr =
 new std::int32_t{0}; // UndCC_Violation - new expression
 // ...
 if (errorCode != 0)
 {
 throw std::runtime_error{"Error"}; // Memory leak could occur here
 }
 // ...

 if (errorCode != 0)
 {
 return 1; // Memory leak could occur here
 }

 // ...
 delete ptr; // UndCC_Violation - delete expression

 return errorCode;
 }
 std::int32_t Fn2()
 {
 std::int32_t errorCode{0};

 std::unique_ptr<std::int32_t> ptr1 = std::make_unique<std::int32_t>(
      0); // UndCC_Valid - alternative for ’new std::int32_t(0)’

 std::unique_ptr<std::int32_t> ptr2(new std::int32_t{0}); // UndCC_Violation - unique_ptr provides make_unique
 // function which shall be used instead of explicit
 // new expression

 std::shared_ptr<std::int32_t> ptr3 =
 std::make_shared<std::int32_t>(0); // UndCC_Valid

 std::vector<std::int32_t> array; // UndCC_Valid
 // alternative for dynamic array

 if (errorCode != 0)
 {
 throw std::runtime_error{"Error"}; // No memory leaks
 }
 // ...
 if (errorCode != 0)
 {
 return 1; // No memory leaks
 }
 // ...
 return errorCode; // No memory leaks
 }
 template <typename T>
 class ObjectManager
 {
 public:
 explicit ObjectManager(T* obj) : object{obj} {}
 ~ObjectManager() { delete object; } // UndCC_Valid by exception
 // Implementation

 private:
 T* object;
 };
 std::int32_t Fn3()
 {
 std::int32_t errorCode{0};

 ObjectManager<std::int32_t> manager{
 new std::int32_t{0}};  // UndCC_Violation
 if (errorCode != 0)
 {
 throw std::runtime_error{"Error"}; // No memory leak
 }
 // ...
 if (errorCode != 0)
 {
 return 1; // No memory leak
 }
 // ...
 return errorCode; // No memory leak
 }
