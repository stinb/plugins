// Minimal std library mocks for testing
namespace std {

template <typename T>
struct vector {
  T *data_;
  T *begin() { return data_; }
  T *end() { return data_; }
};

template <typename T, typename Compare = void>
struct set {};

template <typename T, typename Compare = void>
struct multiset {};

template <typename Key, typename T, typename Compare = void>
struct map {};

template <typename Key, typename T, typename Compare = void>
struct multimap {};

template <typename T, typename Container = void, typename Compare = void>
struct priority_queue {};

template <typename Iter, typename Pred>
bool any_of(Iter first, Iter last, Pred p) { return p(*first); }

template <typename Iter, typename Pred>
bool all_of(Iter first, Iter last, Pred p) { return p(*first); }

template <typename Iter, typename Pred>
bool none_of(Iter first, Iter last, Pred p) { return p(*first); }

template <typename Iter, typename Pred>
Iter find_if(Iter first, Iter last, Pred p) { return first; }

template <typename Iter, typename Pred>
int count_if(Iter first, Iter last, Pred p) { return 0; }

template <typename Iter, typename Pred>
Iter remove_if(Iter first, Iter last, Pred p) { return first; }

template <typename Iter, typename Compare>
void sort(Iter first, Iter last, Compare comp) {}

template <typename Iter>
void sort(Iter first, Iter last) {}

template <typename Iter, typename Compare>
void stable_sort(Iter first, Iter last, Compare comp) {}

template <typename Iter, typename Pred>
Iter partition(Iter first, Iter last, Pred p) { return first; }

template <typename Iter, typename Pred>
bool is_sorted(Iter first, Iter last, Pred p) { return true; }

template <typename Iter, typename Pred>
Iter unique(Iter first, Iter last, Pred p) { return first; }

template <typename Iter, typename Pred>
Iter adjacent_find(Iter first, Iter last, Pred p) { return first; }

template <typename Iter, typename Pred>
Iter copy_if(Iter first, Iter last, Iter out, Pred p) { return out; }

template <typename Iter, typename Func>
void for_each(Iter first, Iter last, Func f) {}

template <typename T, typename Compare>
const T &min(const T &a, const T &b, Compare comp) { return a; }

template <typename T, typename Compare>
const T &max(const T &a, const T &b, Compare comp) { return a; }

} // namespace std

using int32_t = int;

// --- Examples from the standard ---

bool bar(std::vector<int32_t> &v, int32_t &count)
{
  return std::any_of(v.begin(), v.end(),
    [&count](int32_t i)       // UndCC_Violation
    {
      if (i == 3) {
        ++count;              // Persistent side effect
        return true;
      }
      return false;
    }
  );
}

struct Comp {
  bool operator()(int32_t a, int32_t b)  // Non-compliant - not const
  {
    return a > b;
  }
};
std::set<int32_t, Comp> mySet;  // UndCC_Violation

// --- Additional violation cases ---

// Mutable lambda as predicate
bool test_mutable_lambda(std::vector<int32_t> &v)
{
  int count = 0;
  return std::any_of(v.begin(), v.end(),
    [count](int32_t i) mutable  // UndCC_Violation - mutable lambda
    {
      ++count;
      return i == 3;
    }
  );
}

// Lambda with ref capture modified
bool test_ref_capture_modified(std::vector<int32_t> &v, int32_t &count)
{
  return std::any_of(v.begin(), v.end(),
    [&count](int32_t i)  // UndCC_Violation - modifies captured ref
    {
      if (i == 3) {
        ++count;
        return true;
      }
      return false;
    }
  );
}

// Lambda with default [&] capture and modification
bool test_default_ref_capture(std::vector<int32_t> &v)
{
  int32_t total = 0;
  return std::all_of(v.begin(), v.end(),
    [&](int32_t i)  // UndCC_Violation - modifies captured ref
    {
      total += i;
      return i > 0;
    }
  );
}

// Functor with non-const operator() in algorithm call
struct BadComp {
  bool operator()(int32_t a, int32_t b)  // non-const
  {
    return a > b;
  }
};

void test_functor_in_sort(std::vector<int32_t> &v)
{
  BadComp comp;
  std::sort(v.begin(), v.end(), comp);  // UndCC_Violation
}

// Container with non-const comparator
struct BadSetComp {
  bool operator()(int32_t a, int32_t b)  // non-const
  {
    return a < b;
  }
};

void test_container_comparator()
{
  std::set<int32_t, BadSetComp> mySet;  // UndCC_Violation
}

// Lambda with compound assignment to ref capture
void test_compound_assignment(std::vector<int32_t> &v)
{
  int32_t sum = 0;
  std::find_if(v.begin(), v.end(),
    [&sum](int32_t i)  // UndCC_Violation - compound assignment to ref capture
    {
      sum += i;
      return sum > 100;
    }
  );
}

// Functor with non-const in stable_sort
void test_functor_stable_sort(std::vector<int32_t> &v)
{
  std::stable_sort(v.begin(), v.end(), BadComp{});  // UndCC_Violation
}

// Non-const functor in map container
void test_map_comparator()
{
  std::map<int32_t, int32_t, BadSetComp> myMap;  // UndCC_Violation
}

// Non-const functor in multiset container
void test_multiset_comparator()
{
  std::multiset<int32_t, BadSetComp> myMultiSet;  // UndCC_Violation
}

// Non-const functor in priority_queue container
void test_priority_queue_comparator()
{
  std::priority_queue<int32_t, void, BadSetComp> myPQ;  // UndCC_Violation
}

// --- Compliant cases ---

// No-capture lambda
void test_no_capture_lambda(std::vector<int32_t> &v)
{
  std::find_if(v.begin(), v.end(),
    [](int32_t i)  // UndCC_Valid
    {
      return i > 5;
    }
  );
}

// Value-capture lambda (const by default, no persistent side effects)
bool test_value_capture(std::vector<int32_t> &v)
{
  int32_t threshold = 10;
  return std::any_of(v.begin(), v.end(),
    [threshold](int32_t i)  // UndCC_Valid
    {
      return i > threshold;
    }
  );
}

// Ref-capture lambda, read-only
bool test_ref_capture_readonly(std::vector<int32_t> &v, const int32_t &limit)
{
  return std::all_of(v.begin(), v.end(),
    [&limit](int32_t i)  // UndCC_Valid - only reads, no modification
    {
      return i < limit;
    }
  );
}

// Const operator() functor
struct GoodComp {
  bool operator()(int32_t a, int32_t b) const
  {
    return a < b;
  }
};

void test_const_functor(std::vector<int32_t> &v)
{
  std::sort(v.begin(), v.end(), GoodComp{});  // UndCC_Valid
}

// Container with const comparator
void test_good_container()
{
  std::set<int32_t, GoodComp> mySet;  // UndCC_Valid
}

// for_each is NOT a predicate algorithm — side effects are allowed
void test_for_each(std::vector<int32_t> &v)
{
  int32_t sum = 0;
  std::for_each(v.begin(), v.end(),
    [&sum](int32_t i)  // UndCC_Valid - for_each is not a predicate
    {
      sum += i;
    }
  );
}

// Sort without comparator (default)
void test_sort_default(std::vector<int32_t> &v)
{
  std::sort(v.begin(), v.end());  // UndCC_Valid
}

// Good comparator in map
void test_good_map()
{
  std::map<int32_t, int32_t, GoodComp> myMap;  // UndCC_Valid
}
