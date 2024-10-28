class Handle_n
{
    class Body_n *impl; // Declaration of a pointer to an incomplete class
public:
    ~Handle_n() { delete impl; } // UndCC_Violation, Deletion of pointer to an incomplete class
                                 // ...
};