// Custom

#ifndef OPAQUE_H
#define OPAQUE_H

typedef struct OpaqueType *pOpaqueType;
typedef struct VisibleType *pVisibleType;
typedef struct VisibleTypeDeref *pVisibleTypeDeref;

struct VisibleType
{
  /* VisibleType Implementation */
  int x;
};

struct VisibleTypeDeref
{
  /* VisibleTypeDeref Implementation */
  int x;
};

#endif
