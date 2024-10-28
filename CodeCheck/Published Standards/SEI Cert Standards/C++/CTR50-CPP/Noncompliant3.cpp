#include <iterator>

template <typename ForwardIterator>
void f_imp(ForwardIterator b, ForwardIterator e, int val, std::forward_iterator_tag)
{
    do
    {
        *b++ = val; // UndCC_Violation
    } while (b != e);
}

template <typename ForwardIterator>
void f(ForwardIterator b, ForwardIterator e, int val)
{
    typename std::iterator_traits<ForwardIterator>::iterator_category cat;
    f_imp(b, e, val, cat);
}

void useTemplate() 
{
    int *a, *b = 0;
    int c = 0;
    f(a,b,c);
}
