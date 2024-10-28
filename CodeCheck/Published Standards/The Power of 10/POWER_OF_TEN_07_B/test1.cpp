static int not_needed_for_bool_cpp(bool x, bool y)
{
  return !x || !y;
}

static bool get_false()
{
  return false;
}

static void unchecked_return_value_cpp()
{
  bool b = get_false();
  b = !b || b;
}
