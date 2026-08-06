// Part-2-only case: the specialization IS in the same file as its primary
// template (satisfies MISRA C++ 2008 14-7-3), but the specialized user-defined
// type Widget is declared in a different file. AUTOSAR A14-7-2 flags this; the
// MISRA C++ 2008 rule does not.
#include "widget.hpp"

template <typename T>
struct Info
{
};

// Same file as the primary template Info -> compliant with 14-7-3.
template <>
struct Info<Widget>
{
};

Info<Widget> gInfo; // UndCC_Violation(full) - Widget declared in widget.hpp
