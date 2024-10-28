#include <stdexcept>

void testf02(); //UNDCC_Violation - the declaration must also inclue the throws in the preamble

/* this is a test
hello world */
void testf00(){ throw std::runtime_error("err"); } //UNDCC_Violation

// @throw runtime_error
// hello world
void testf01(){throw std::runtime_error("err");} // UNDCC_Valid

// @throw runtime_error
void testf02(){throw std::runtime_error("err");} // UNDCC_Valid
