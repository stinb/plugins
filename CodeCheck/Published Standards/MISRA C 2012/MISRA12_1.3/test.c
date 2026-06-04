/* Test for MISRA C 2012/2023/2025 Rule 1.3 - There shall be no occurrence of
 * undefined or critical unspecified behaviour.
 *
 * Rule 1.3 is a catch-all. This is a conservative subset that flags only three
 * statically-decidable residual undefined behaviours that Appendix H lists with
 * no covering guideline:
 *
 *   - J.2 #4  : main defined with a return type other than int (hosted).
 *   - J.2 #60 : a struct or union defined with no named members (C only).
 *   - J.2 #63 : a const object modified through a non-const lvalue, in the
 *               decidable form of a write through a cast applied directly to
 *               the address of (or a reference to) a declared const object.
 *
 * Lines with no violation marker are expected to produce no violation, either
 * because they are compliant or because they are documented false negatives.
 */

/* ---- J.2 #60: struct or union with no named members --------------------- */

struct empty_struct { };                       /* UndCC_Violation */
union  empty_union  { };                        /* UndCC_Violation */
struct only_unnamed_bitfields { int : 4; unsigned : 2; }; /* UndCC_Violation */

struct has_member { int x; };                   /* UndCC_Valid */
struct named_bitfield { unsigned f : 3; };      /* UndCC_Valid */
union  has_member_u { int a; double b; };       /* UndCC_Valid */
struct fwd_only;                                /* UndCC_Valid - not a definition */
struct with_anon_member { union { int via_anon; }; };  /* UndCC_Valid - named via anon union */
struct named_field_only { struct { int inner; } s; };  /* UndCC_Valid - field s is named */

/* ---- J.2 #63: modifying a const object through a non-const lvalue -------- */

static const int kConstInt = 1;
static int gMutableInt = 0;

struct Obj { int m; };
static const struct Obj kConstObj = { 0 };

static void const_modification_tests ( void )
{
    *( int * ) &kConstInt = 2;             /* UndCC_Violation - write to const object   */
    ( ( int * ) &kConstInt )[ 0 ] = 3;     /* UndCC_Violation - write via subscript     */
    ( *( int * ) &kConstInt )++;           /* UndCC_Violation - increment const object  */

    *( int * ) &gMutableInt = 5;           /* UndCC_Valid - object is not const         */
    gMutableInt = 6;                       /* UndCC_Valid - ordinary write              */

    /* Documented false negatives (no value-flow analysis): */
    ( ( struct Obj * ) &kConstObj )->m = 7; /* member through pointer - not detected    */
    {
        const int *p = &kConstInt;
        *( int * ) p = 8;                  /* write through a pointer variable          */
    }
}

/* ---- J.2 #4: main not defined with a return type of int ----------------- */

void main ( void )                         /* UndCC_Violation - return type is not int  */
{
    const_modification_tests ( );
}
