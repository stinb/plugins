struct S
{
  int x;
  int y;
};
struct T
{
  int w;
  struct S s;
  int z;
};
struct T tt = { /* UndCC_Violation - chained designators and implicit positional initializers mixed */
  1,
  .s.x = 2, /* To a human reader, this looks like .z is being initialized */
  3, /* tt is actually initialized as { 1, { 2, 3 }, 0 } */
}; /* This also violates Rule 9.2 */
/* Compliant - allow the y dimension to implicitly initialize to zero */
struct S aa[5] = {
[0].x = 1,
[1].x = 2,
[2].x = 3,
[3].x = 4,
[4].x = 5,
};
/* Compliant - the initializer for [1] is not chained, but is explicit */
struct S ab[2] = {
[0].x = 1,
[1] = { 2, 3 }, /* Compliant by exception: */
}; /* the positional initializers are inside a braced sub-object */
