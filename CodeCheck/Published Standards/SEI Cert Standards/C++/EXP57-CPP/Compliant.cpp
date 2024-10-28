class Handle
{
    class Body *impl; // Declaration of a pointer to an incomplete class
public:
    ~Handle();
    // ...
};

// Elsewhere
class Body
{ /* ... */
};

Handle::~Handle()
{
    delete impl;
}