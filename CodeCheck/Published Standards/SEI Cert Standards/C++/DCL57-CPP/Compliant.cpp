#include "Bad.h"

class SomeClass
{
    Bad bad_member;

public:
    ~SomeClass()
    try
    {
        // ...
    }
    catch (...)
    {
        // Catch exceptions thrown from noncompliant destructors of
        // member objects or base class subobjects.

        // NOTE: Flowing off the end of a destructor function-try-block causes
        // the caught exception to be implicitly rethrown, but an explicit
        // return statement will prevent that from happening.
        return;
    }
};