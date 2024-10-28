#include <cstdint>
class A
{
public:
	void SetProperty(std::int32_t x) noexcept { property = x; }
	void DoSomething() noexcept {}

private:
	std::int32_t property;
};
struct B
{
};
class C
{
public:
	void DoSomething() noexcept {}
};
template <typename T>
class D
{
public:
	void F1() {}
	void F2()
	{
		T t;
		t.SetProperty(0);
	}
	void F3()
	{
		T t;
		t.DoSomething();
	}
};

void Fn() noexcept
{
	D<A> d1; // UndCC_Valid - struct A provides all needed members
	d1.F1();
	d1.F2();
	d1.F3();

	D<B> d2; // UndCC_Violation - struct B does not provide needed members
	d2.F1();
	// d2.f2(); // Compilation error - no 'property' in struct B
	// d2.f3(); // Compilation error - no member named 'DoSomething' in struct B

	D<C> d3; // UndCC_Violation - struct C does not provide property
	d3.F1();
	// d3.F2(); // Compilation error - no property in struct C
	d3.F3();
}
