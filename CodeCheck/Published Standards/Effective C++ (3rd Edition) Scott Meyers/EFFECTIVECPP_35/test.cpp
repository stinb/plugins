class Test {
public:
virtual int test() const; // UNDCC_Violation
};


// UNDCC_Valid
class GameCharacter; 
int defaultHealthCalc(const GameCharacter& gc);
class GameCharacter {
public:
  typedef int (*HealthCalcFunc)(const GameCharacter&);
  explicit GameCharacter(HealthCalcFunc hcf = defaultHealthCalc) : healthFunc(hcf )
  {}
  int healthValue() const
  { return healthFunc(*this); }
  
private:
  HealthCalcFunc healthFunc;
};
