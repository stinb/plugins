// Issue #4935: definitions for the templates declared in
// templateParametersInHeader.h. Parameter names match the declarations, so
// nothing here should be flagged.
#include "templateParametersInHeader.h"

template <typename T>
bool getUniqueMax( const VoteItem<T> VoteItemList[MaxVoteItems], const uint32_t ListSize, uint32_t &idxOfMax ) // UndCC_Valid
{
    return false;
}

template <typename T>
bool vote( VoteItem<T> voteItemList[MaxVoteItems], const uint32_t ListSize, uint32_t &majorityIdx ) // UndCC_Valid
{
    return false;
}

template <typename T>
void excludeNonMajorityItems( VoteItem<T> voteItemList[MaxVoteItems], const uint32_t ListSize, const uint32_t IdxOfMajority ) // UndCC_Valid
{
}
