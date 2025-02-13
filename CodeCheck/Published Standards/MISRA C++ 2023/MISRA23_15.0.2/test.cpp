struct UniqueManager
{
    UniqueManager() = default;
    UniqueManager(UniqueManager &&) noexcept;            // UndCC_Valid - move constructor
    UniqueManager &operator=(UniqueManager &&) noexcept; // UndCC_Violation - needs & qualifier - move assignment
};

struct Manager
{
    Manager(Manager const &other) noexcept(false);   // UndCC_Valid - copy constructor
    Manager(const Manager &other, char c);           // Not a copy-constructor
    Manager(Manager &&other, char c = 'x') noexcept; // UndCC_Violation, has another parameter with default value - move constructor
};

struct ScopedManager
{
    ScopedManager();
    ~ScopedManager();
    ScopedManager &operator=(ScopedManager &&) = delete; // Rule does not apply, has delete
};

struct Bad
{
    Bad(Bad volatile const &);             // UndCC_Violation - volatile, copy constructor
    virtual Bad &operator=(Bad const &) &; // UndCC_Violation - virtual, copy assignment
};
