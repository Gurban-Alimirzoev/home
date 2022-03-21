#include "single-linked-list.h"

using namespace std;

SingleLinkedList::SingleLinkedList()
{
}

SingleLinkedList::SingleLinkedList(const SingleLinkedList &other)
    : SingleLinkedList(other.begin(), other.end())
{
}

template <typename iter>
SingleLinkedList::SingleLinkedList(iter beginIter, iter endIter)
{
    SingleLinkedList copy;
    for (auto i = beginIter; i != endIter; i++)
    {
        copy.PushFront(*i);
    }
    SingleLinkedList copy_copy;
    for (auto i = copy.begin(); i != copy.end(); i++)
    {
        copy_copy.PushFront(*i);
    }
    swap(copy_copy);
}

SingleLinkedList::SingleLinkedList &operator=(const SingleLinkedList &rhs)
{
    SingleLinkedList copy(rhs);
    swap(copy);
    return *this;
}
