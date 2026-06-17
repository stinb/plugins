/* Examples adapted from MISRA C 2012 / 2023 / 2025 Rule 20.8 */

#define FALSE 0
#define TRUE  1

/* Compliant - object-like macro expanding to the literal 0 */
#if FALSE                               // UndCC_Valid
#endif

/* Compliant - object-like macro expanding to the literal 1 */
#if TRUE                                // UndCC_Valid
#endif

/* Non-compliant - integer constant other than 0 or 1 */
#if 10                                  // UndCC_Violation
#endif

/* Compliant - logical NOT of a defined() operator is essentially Boolean */
#if ! defined ( X )                     // UndCC_Valid
#endif

/* Compliant - relational operator yields 0 or 1 */
#if A > B                               // UndCC_Valid
#endif

/* Compliant - the canonical 0 and 1 controlling expressions */
#if 0                                   // UndCC_Valid
#endif
#if 1                                   // UndCC_Valid
#endif

/* Non-compliant - literal 2 */
#if 2                                   // UndCC_Violation
#endif

/* Non-compliant - hexadecimal literal whose value is 16 */
#if 0x10                                // UndCC_Violation
#endif

/* Compliant - 0 and 1 with integer suffixes */
#if 0L                                  // UndCC_Valid
#endif
#if 1u                                  // UndCC_Valid
#endif

/* Non-compliant - macro expanding to a value that is neither 0 nor 1 */
#define VERSION 3
#if VERSION                             // UndCC_Violation
#endif

/* Compliant - macro expanding to 1 */
#define ENABLED 1
#if ENABLED                             // UndCC_Valid
#endif

/* Compliant - the defined() operator is essentially Boolean */
#if defined ( ENABLED )                 // UndCC_Valid
#endif

/* Compliant - bare `defined X` form */
#if defined VERSION                     // UndCC_Valid
#endif

/* Compliant - logical and equality operators yield 0 or 1 */
#if defined(ENABLED) && VERSION == 3    // UndCC_Valid
#endif

/* Compliant - the lowest-precedence top-level operator (>=) is relational,
   so the whole expression is essentially Boolean despite the arithmetic */
#define VER_MAJOR 3
#define VER_MINOR 5
#if VER_MAJOR * 100 + VER_MINOR >= 305  // UndCC_Valid
#endif

/* Non-compliant - top-level operator is arithmetic; result is not 0 or 1 */
#if VER_MAJOR + VER_MINOR               // UndCC_Violation
#endif

/* Non-compliant - top-level operator is multiplicative */
#define SCALE 4
#if SCALE * 2                           // UndCC_Violation
#endif

/* Non-compliant - bitwise AND is not essentially Boolean */
#define FLAGS 6
#define MASK  1
#if FLAGS & MASK                        // UndCC_Violation
#endif

/* Non-compliant - shift operator is not essentially Boolean */
#if 1 << 0                              // UndCC_Violation
#endif

/* Compliant - undefined identifier evaluates to 0 in the preprocessor */
#if SOME_UNDEFINED_FLAG                 // UndCC_Valid
#endif

/* Compliant - macro expanding to a parenthesized Boolean expression */
#define READY ( ENABLED && TRUE )
#if READY                               // UndCC_Valid
#endif

/* Non-compliant - integer constant in an #elif controlling expression */
#if defined ( UNSET )                   // UndCC_Valid
#elif 7                                 // UndCC_Violation
#endif

/* Compliant - function-like macro invocation forming the whole controlling
   expression: arguments are substituted on expansion and cannot be resolved
   statically, so it is not reported */
#ifndef __has_attribute
#define __has_attribute(x) 0
#endif
#if (defined(__GNUC__) && __GNUC__ >= 4) || __has_attribute(sentinel)  // UndCC_Valid
#endif

/* Compliant - a single function-like macro invocation cannot be resolved
   statically, so it is not reported */
#if __has_attribute ( nonnull )         // UndCC_Valid
#endif

/* Compliant - #ifdef and #ifndef are not controlling expressions in the
   0-or-1 sense and are not subject to this rule */
#ifdef NEVER_DEFINED                    // UndCC_Valid
#endif
#ifndef ALSO_NEVER_DEFINED              // UndCC_Valid
#endif

/* Compliant - line-continued controlling expression whose top-level
   operator is logical AND */
#if ENABLED == 1 && \
    VERSION == 3
#endif

/* Compliant - relational operator split across a line continuation. If the
   backslash-newline were not joined, the first line alone ("#if VERSION")
   would expand to 3 and be wrongly reported. */
#if VERSION \
    == 3
#endif

/* Limitation - the Amplification exempts expressions in excluded code, but a
   nested directive in an inactive #if 0 block would still be reported. No
   live example is included so this behavior is not treated as expected:

       #if 0
       #if 10     // would be reported, though the rule does not apply
       #endif
       #endif
*/

/* Compliant - an integer literal in user code, outside any #if/#elif,
   must NOT be flagged */
int not_a_directive = 10;               // UndCC_Valid
