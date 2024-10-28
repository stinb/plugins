#include <string>

// This class should flag 3 violations: missing destructor, missing copy constructor, and missing assignment operator
class Car { // UndCC_Violation
  public:
    std::string brand;   
    std::string model;
    int year;
    int * p_num_owners = new int[10];
};
