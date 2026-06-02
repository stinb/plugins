#ifndef STRING_LIB_H
#define STRING_LIB_H

extern void *memset( void *s, int c, unsigned long n );

/* Fortified library headers (e.g. glibc under _FORTIFY_SOURCE) redirect memset
   to a compiler builtin. The expansion introduces __builtin___memset_chk and
   __builtin_object_size, which the strict parser records as implicitly declared
   at the call site even though they are compiler intrinsics, not user code. */
#define memset( s, c, n ) __builtin___memset_chk ( s, c, n, __builtin_object_size ( s, 0 ) )

#endif
