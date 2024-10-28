// Custom

#include <algorithm>
#include <vector>

bool shouldCopy(int i) {
	return i < 0;
}

void f(const std::vector<int> &src) {
  std::vector<int> dest(src.size());
  std::copy_if(src.begin(), src.end(), dest.begin(), shouldCopy); // UndCC_Valid
}

void g(const std::vector<int> &src) {
  std::vector<int> dest;
  std::copy_if(src.begin(), src.end(), dest.begin(), shouldCopy); // UndCC_Violation(Win,Lin)
}
