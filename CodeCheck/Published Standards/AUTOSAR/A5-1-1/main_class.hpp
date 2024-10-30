#include <cstddef>

static const unsigned int j2[256lu] = { // UndCC_Valid - array size
	0x00000000, 0xab59b4d1, 0x56b369a2, 0xfdeadd73, 0x063f6795, 0xad66d344,
	0x508c0e37, 0xfbd5bae6, 0x0c7ecf2a, 0xa7277bfb, 0x5acda688, 0xf1941259
}; // UndCC_Valid - assignment


class main_class
{
	private:
		float g1 = 0.1f; // UndCC_Valid - assignment
		long double g2 = 0.1L; // UndCC_Valid - assignment
		double g3 = 0.0; // UndCC_Valid - assignment
		long int g4 = NULL;
		char g5 = '&';
		char* g6 = NULL;
		int g7 = NULL;
		int g8 = 0u; // UndCC_Valid - assignment
		double long g9 = 0.1L; // UndCC_Valid - assignment
	public:
		main_class();
		main_class(float f_value,long int li_value);
		float get_g1();
		void set_g1(float f_value);
		long double get_g2();
		void set_g2(long double ld_value);
		void check_value(wchar_t string5[]);
};