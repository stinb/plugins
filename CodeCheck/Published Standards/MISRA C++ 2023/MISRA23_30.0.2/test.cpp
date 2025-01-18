#include <fstream>

void show_fstream_non_compliant()
{
  std::fstream f{"hello.txt"};
  std::basic_filebuf<char> filebuf;
  f << "Hello world!\n"
    << std::flush; // flush is not a positioning operation
  std::string s{};
  std::getline(f, s); // UndCC_Violation - undefined behaviour
}
void show_fstream_compliant()
{
  std::fstream f{"hello.txt"};
  f << "Hello world!\n";
  std::string s{};
  f.seekg(0, std::ios_base::beg);
  std::getline(f, s); // UndCC_Valid - s holds "Hello world!"
}
