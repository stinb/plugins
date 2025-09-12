// wrap_ptr.h
#include "wrap.h"
template <typename V>
struct wrap<V *> // UndCC_Violation - should be in wrap.h
{
}; 
