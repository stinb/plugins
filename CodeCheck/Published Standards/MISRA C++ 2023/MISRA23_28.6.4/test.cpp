namespace std {
  int *remove(int *first, int *last, const int &val);
  int remove(const char *path);

  template <typename It, typename Pred>
  It remove_if(It first, It last, Pred pred);

  template <typename It>
  It unique(It first, It last);

  template <typename C>
  bool empty(const C &c);
}

struct Vec {
  int *b, *e;
  int *begin() { return b; }
  int *end() { return e; }
  void erase(int *first, int *last);
  void push_back(int val);
  bool empty() const;
};

// std::remove with result discarded
void test_remove_discarded(Vec &v) {
  std::remove(v.begin(), v.end(), 2);              // UndCC_Violation
}

// std::remove with result used in erase
void test_remove_used(Vec &v) {
  v.erase(std::remove(v.begin(), v.end(), 2), v.end()); // UndCC_Valid
}

// std::remove with result assigned
void test_remove_assigned(Vec &v) {
  auto it = std::remove(v.begin(), v.end(), 2);    // UndCC_Valid
  v.erase(it, v.end());
}

// std::remove_if with result discarded
bool pred(int x);
void test_remove_if_discarded(Vec &v) {
  std::remove_if(v.begin(), v.end(), pred);         // UndCC_Violation
}

// std::remove_if with result used
void test_remove_if_used(Vec &v) {
  v.erase(std::remove_if(v.begin(), v.end(), pred), v.end()); // UndCC_Valid
}

// std::unique with result discarded
void test_unique_discarded(Vec &v) {
  std::unique(v.begin(), v.end());                  // UndCC_Violation
}

// std::unique with result used in erase
void test_unique_used(Vec &v) {
  v.erase(std::unique(v.begin(), v.end()), v.end()); // UndCC_Valid
}

// .empty() member call with result discarded
void test_empty_member_discarded(Vec &v) {
  v.empty();                                        // UndCC_Violation
}

// .empty() result used in condition
void test_empty_member_used(Vec &v) {
  if (v.empty()) {                                  // UndCC_Valid
    v.push_back(1);
  }
}

// .empty() result assigned
void test_empty_assigned(Vec &v) {
  bool b = v.empty();                               // UndCC_Valid
  (void)b;
}

// std::empty() free function with result discarded
void test_std_empty_discarded(Vec &v) {
  std::empty(v);                                    // UndCC_Violation
}

// std::empty() free function with result used
void test_std_empty_used(Vec &v) {
  if (std::empty(v)) {                              // UndCC_Valid
    v.push_back(1);
  }
}

// std::remove(const char*) from <cstdio> — should NOT be flagged
void test_cstdio_remove() {
  std::remove("/tmp/test.txt");                     // UndCC_Valid
}

// Cast to void — should NOT be flagged
void test_void_cast(Vec &v) {
  (void)std::remove(v.begin(), v.end(), 2);         // UndCC_Valid
}
