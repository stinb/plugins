#include <iterator>

template <typename ForwardIterator>
void f_imp(ForwardIterator b, ForwardIterator e, int val, std::forward_iterator_tag)
{
    while (b != e)
    {
        *b++ = val;
    }
}

template <typename ForwardIterator>
void f(ForwardIterator b, ForwardIterator e, int val)
{
    typename std::iterator_traits<ForwardIterator>::iterator_category cat;
    f_imp(b, e, val, cat);
}