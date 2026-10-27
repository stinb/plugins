struct Group { int target; };
struct Vec { Group d[4]; Group* begin() { return d; } Group* end() { return d + 4; } };

class Widget
{
public:
  void update();
private:
  template<typename Target_T>
  Group* buttonGroup(const Target_T Target);  // UndCC_Valid
  template<typename T, typename U>
  void unused(T, U);                            // UndCC_Violation
  Vec m_groups;
};

template<typename T, typename V> bool holds(const V&) { return true; }  // UndCC_Valid
