#include <memory>
#include <functional>

class my_class {
public:
  // ...
  typedef std::function<void()> callback;
  void on_complete(callback cb) { complete_callback = cb; }
  void clean_something_up() {} // Declare this method
private:
  callback complete_callback;
  // ...
};

int main() {
  // ...
  std::shared_ptr<my_class> obj = std::make_shared<my_class>();
  std::shared_ptr<my_class> obj1 = std::make_shared<my_class>();
  std::weak_ptr<my_class> weak_obj = obj;
  obj->on_complete([obj]() { // UndCC_Violation(0)
    obj->clean_something_up();
  });
  obj->on_complete([obj1]() { // UndCC_Valid
    obj1->clean_something_up();
  });
  // executor->submit(obj); // You'll need to define executor
  // ...
  return 0;
}
