class Engine {
public:
Engine(int numCylinders);
void start();
};

// UndCC_Valid
class Car {
public:
  Car() : e_(8) { }
  void start() { e_.start(); }
private:
  Engine e_;
};


class Car1 : private Engine { // UndCC_Violation
public:
  Car1() : Engine(8) { }
  using Engine::start;
};
;
