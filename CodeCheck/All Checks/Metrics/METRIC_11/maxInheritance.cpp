#include <iostream>

#include <iostream>

// Base class
class Base {
public:
    Base() {
        std::cout << "Base constructor" << std::endl;
    }

    virtual ~Base() {
        std::cout << "Base destructor" << std::endl;
    }
};

// Level 1
class Level1 : public Base {
public:
    Level1() {
        std::cout << "Level1 constructor" << std::endl;
    }

    virtual ~Level1() {
        std::cout << "Level1 destructor" << std::endl;
    }
};

// Level 2
class Level2 : public Level1 {
public:
    Level2() {
        std::cout << "Level2 constructor" << std::endl;
    }

    virtual ~Level2() {
        std::cout << "Level2 destructor" << std::endl;
    }
};

// Level 3
class Level3 : public Level2 {
public:
    Level3() {
        std::cout << "Level3 constructor" << std::endl;
    }

    virtual ~Level3() {
        std::cout << "Level3 destructor" << std::endl;
    }
};

// Level 4
class Level4 : public Level3 {
public:
    Level4() {
        std::cout << "Level4 constructor" << std::endl;
    }

    virtual ~Level4() {
        std::cout << "Level4 destructor" << std::endl;
    }
};

// Level 5
class Level5 : public Level4 {
public:
    Level5() {
        std::cout << "Level5 constructor" << std::endl;
    }

    virtual ~Level5() {
        std::cout << "Level5 destructor" << std::endl;
    }
};

// Level 6
class Level6 : public Level5 { // UndCC_Violation
public:
    Level6() {
        std::cout << "Level6 constructor" << std::endl;
    }

    virtual ~Level6() {
        std::cout << "Level6 destructor" << std::endl;
    }
};

int main() {
    // Create an instance of the most derived class (Level6)
    Level6 obj;

    return 0;
}
