// Issue #4935: template functions taking an array of a template-type
// parameter, declared in a header and defined in a separate source file.
// Parameter names are identical between declaration and definition, so the
// declarations below must not be flagged. (Understand attaches a spurious
// reference to the template argument 'T' here; the check must ignore it.)
#ifndef TEMPLATE_PARAMETERS_IN_HEADER_H
#define TEMPLATE_PARAMETERS_IN_HEADER_H

typedef unsigned int uint32_t;

const uint32_t MaxVoteItems = 8;

template<class T> struct VoteItem
{
    T value;
    uint32_t count;
};

template<class T> bool vote( VoteItem<T> voteItemList[MaxVoteItems], // UndCC_Valid
                             const uint32_t ListSize,
                             uint32_t &majorityIdx );
template<class T> void excludeNonMajorityItems( VoteItem<T> voteItemList[MaxVoteItems], // UndCC_Valid
                                                const uint32_t ListSize,
                                                const uint32_t IdxOfMajority );
template<class T> bool getUniqueMax( const VoteItem<T> VoteItemList[MaxVoteItems], // UndCC_Valid
                                     const uint32_t ListSize,
                                     uint32_t &idxOfMax );

#endif
