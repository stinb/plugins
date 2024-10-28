// Custom

class Functor {
  public:
    void operator()() {}
};

template <typename Predicate>
void foo(Predicate predicate) // UndCC_Valid
{
  predicate();
}

template <typename Predicate>
void bar(Predicate predicate) // UndCC_Violation
{

}

int main()
{
  Functor f;
  foo(f);
  bar(f);
}
