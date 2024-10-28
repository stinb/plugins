// SEI CERT C++

#ifndef FILE_H
#define FILE_H

class Car {
  int numWheels;

public:
  Car() : numWheels(4) {}
  explicit Car(int numWheels) : numWheels(numWheels) {}

  int get_num_wheels() const { return numWheels; }
};
#endif // FILE_H
