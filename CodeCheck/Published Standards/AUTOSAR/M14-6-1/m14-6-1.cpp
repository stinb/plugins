#include <cstdint>

typedef int32_t TYPE;
void g ( );
template <typename T>
class B;
template <typename T>
class A : public B<T>
{
	void f1 ( )
	{
		TYPE t = 0;                 // UndCC_Violation Example 1
		g ( );                      // UndCC_Violation Example 2
	}
	void f2 ( )
	{
		::TYPE t1 = 0;              // UndCC_Valid - explicit use global TYPE
		::g ( );                    // UndCC_Valid - explicit use global func
		typename B<T>::TYPE t2 = 0; // UndCC_Valid - explicit use base TYPE
		this->g ( );                // UndCC_Valid - explicit use base "g"
	}
};
template <typename T>
class B
{
public:
	typedef T TYPE;
	void g ( );
};
template class A<int32_t>;
