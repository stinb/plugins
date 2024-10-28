// Custom

void modify_const_ref(const int &const_ref)
{
  int &ref = (int &) const_ref; // UndCC_Violation - c-style cast
  ref = 42;
}

void call_modify_const_ref()
{
  const int num = 4;
  modify_const_ref(num);
}

void legacy_api_use(int &num)
{
  int other_num = num;
}

void call_legacy_api_use()
{
  const int num = 1;
  legacy_api_use(const_cast<int &>(num)); // UndCC_Valid - good cast for legacy API
}

void legacy_api_assign(int unused, int &num)
{
  num = 2;
}

void call_legacy_api_assign()
{
  int unused = 0;
  const int num = 1;
  legacy_api_assign(unused, const_cast<int &>(num)); // UndCC_Violation - bad cast for legacy API
}

void legacy_api_modify(int &num, int unused)
{
  num++;
}

void call_legacy_api_modify()
{
  int unused = 0;
  const int num = 0;
  legacy_api_assign(const_cast<int &>(num), unused); // UndCC_Violation - bad cast for legacy API
}
