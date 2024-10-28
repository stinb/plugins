#include <memory>

class Handle
{
    std::shared_ptr<class Body> impl;

public:
    Handle();
    ~Handle() {}
    // ...
};