// Custom

#include <functional>
#include <map>
#include <queue>
#include <set>
#include <vector>

void set() {
  std::set<int, std::equal_to<int>> a;      // UndCC_Violation(Win,Lin)
  std::set<int, std::not_equal_to<int>> b;  // UndCC_Violation(Win,Lin)
  std::set<int, std::greater<int>> c;       // UndCC_Valid
  std::set<int, std::less<int>> d;          // UndCC_Valid
  std::set<int, std::greater_equal<int>> e; // UndCC_Violation(Win,Lin)
  std::set<int, std::less_equal<int>> f;    // UndCC_Violation(Win,Lin)
}

void map() {
  std::map<int, int, std::equal_to<int>> a;      // UndCC_Violation(Win,Lin)
  std::map<int, int, std::not_equal_to<int>> b;  // UndCC_Violation(Win,Lin)
  std::map<int, int, std::greater<int>> c;       // UndCC_Valid
  std::map<int, int, std::less<int>> d;          // UndCC_Valid
  std::map<int, int, std::greater_equal<int>> e; // UndCC_Violation(Win,Lin)
  std::map<int, int, std::less_equal<int>> f;    // UndCC_Violation(Win,Lin)
}

void multiset() {
  std::multiset<int, std::equal_to<int>> a;      // UndCC_Violation(Win,Lin)
  std::multiset<int, std::not_equal_to<int>> b;  // UndCC_Violation(Win,Lin)
  std::multiset<int, std::greater<int>> c;       // UndCC_Valid
  std::multiset<int, std::less<int>> d;          // UndCC_Valid
  std::multiset<int, std::greater_equal<int>> e; // UndCC_Violation(Win,Lin)
  std::multiset<int, std::less_equal<int>> f;    // UndCC_Violation(Win,Lin)
}

void multimap() {
  std::multimap<int, int, std::equal_to<int>> a;      // UndCC_Violation(Win,Lin)
  std::multimap<int, int, std::not_equal_to<int>> b;  // UndCC_Violation(Win,Lin)
  std::multimap<int, int, std::greater<int>> c;       // UndCC_Valid
  std::multimap<int, int, std::less<int>> d;          // UndCC_Valid
  std::multimap<int, int, std::greater_equal<int>> e; // UndCC_Violation(Win,Lin)
  std::multimap<int, int, std::less_equal<int>> f;    // UndCC_Violation(Win,Lin)
}

void priority_queue() {
  std::priority_queue<int, std::vector<int>, std::equal_to<int>> a;      // UndCC_Violation(Win,Lin)
  std::priority_queue<int, std::vector<int>, std::not_equal_to<int>> b;  // UndCC_Violation(Win,Lin)
  std::priority_queue<int, std::vector<int>, std::greater<int>> c;       // UndCC_Valid
  std::priority_queue<int, std::vector<int>, std::less<int>> d;          // UndCC_Valid
  std::priority_queue<int, std::vector<int>, std::greater_equal<int>> e; // UndCC_Violation(Win,Lin)
  std::priority_queue<int, std::vector<int>, std::less_equal<int>> f;    // UndCC_Violation(Win,Lin)
}
