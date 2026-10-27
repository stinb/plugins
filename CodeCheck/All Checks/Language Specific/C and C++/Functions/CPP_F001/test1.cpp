#include "test1.h"

template<typename Target_T>
Group* Widget::buttonGroup(const Target_T Target)
{
  int unusedLocal;                       // UndCC_Valid
  for (Group& group : m_groups)          // UndCC_Valid
  {
    if (holds<Target_T>(group.target) && group.target == Target)
    {
      return &group;
    }
  }
  return nullptr;
}

template<typename T, typename U>
void Widget::unused(T, U)                // UndCC_Violation
{
}

void Widget::update()
{
  buttonGroup(1);
  buttonGroup(2);
}
