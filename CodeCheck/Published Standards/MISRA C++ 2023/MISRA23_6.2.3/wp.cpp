// wp.cpp
#include "wrap_ptr.h" // Specialization visible
wrap<char *> b_wrap;  // ODR violation - see w.cpp