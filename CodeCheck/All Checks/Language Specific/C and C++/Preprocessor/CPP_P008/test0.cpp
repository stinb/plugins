// Effective C++


#define ASPECT_RATIO 1.653 // UndCC_Violation

const double AspectRatio = 1.653; // uppercase names are usually for
                                  // macros, hence the name change


// call f with the maximum of a and b
#define CALL_WITH_MAX(a, b) f((a) > (b) ? (a) : (b)) // UndCC_Violation

template<typename T>                            // because we don't
inline void callWithMax(const T& a, const T& b) // know what T is, we
{                                               // pass by reference-to-
  f(a > b ? a : b);                             // const - see Item 20
}
