/* Examples adapted from MISRA C 2012 / 2025 Rule 20.9 */

/* Compliant - defined() guard, no evaluation of M */
#if defined ( M )                                       // UndCC_Valid
#endif

/* Non-compliant - M is undefined and used in a controlling expression */
#if M == 0                                              // UndCC_Violation
#endif

/* Compliant - bare `defined M` form */
#if defined M                                           // UndCC_Valid
#endif

/* Compliant - short-circuit guard: B only evaluated if it is defined */
#if defined ( B ) && ( B == 0 )                         // UndCC_Valid
#endif

/* Compliant - the macro is #define'd in this file before evaluation */
#define HAS_FOO 1
#if HAS_FOO == 1                                        // UndCC_Valid
#endif

/* Compliant - identifier used in #elif after being defined */
#define VERSION 2
#if VERSION == 1                                        // UndCC_Valid
#elif VERSION == 2                                      // UndCC_Valid
#endif

/* Non-compliant - undefined identifier in #elif controlling expression */
#define KNOWN 1
#if KNOWN == 0                                          // UndCC_Valid
#elif UNKNOWN == 1                                      // UndCC_Violation
#endif

/* Compliant - predefined identifier permitted by the rule's amplification */
#if __STDC_VERSION__ >= 199901L                         // UndCC_Valid
#endif

#if __cplusplus >= 201103L                              // UndCC_Valid
#endif

#if __GNUC__ >= 4                                       // UndCC_Valid
#endif

/* Compliant - constant expression with no identifiers */
#if 1 + 2 * 3 > 0                                       // UndCC_Valid
#endif

/* Non-compliant - two undefined identifiers, both flagged */
#if MISSING_A && MISSING_B                              // UndCC_Violation
#endif

/* Compliant - #ifdef and #ifndef are not subject to this rule */
#ifdef NEVER_DEFINED                                    // UndCC_Valid
#endif
#ifndef ALSO_NEVER_DEFINED                              // UndCC_Valid
#endif

/* Compliant - mix of defined() and a properly-defined macro */
#define READY 1
#if defined(READY) && READY == 1                        // UndCC_Valid
#endif

/* Non-compliant - mix of defined() guard and an undefined identifier
   used unconditionally on the right of && */
#if defined(READY) && OOPS                              // UndCC_Violation
#endif

/* Compliant - line-continued controlling expression */
#define COND_A 1
#define COND_B 2
#if COND_A == 1 && \
    COND_B == 2                                         // UndCC_Valid
#endif

/* Non-compliant - line-continued, with an undefined identifier on the
   continuation line */
#if COND_A == 1 && \
    GHOST == 0                                          // UndCC_Violation
#endif

/* Compliant - function-like macro call: arguments are tokens to the macro,
   not identifiers in the post-expansion controlling expression. */
#ifndef __has_attribute
#define __has_attribute(x) 0
#endif
#if (defined(__GNUC__) && __GNUC__ >= 4) || __has_attribute(sentinel)  // UndCC_Valid
#endif

/* Compliant - identifier in user code outside any #if/#elif must NOT be
   flagged as a regression. */
int undefined_in_code_only = 0;                         // UndCC_Valid

/* Compliant - inverse-defined guard idiom: the #if branch handles the
   undefined case (real code uses #error here), so the #elif is only
   reachable when MOC_REV is defined. Common in Qt moc-generated
   headers. */
#if !defined(MOC_REV)
typedef int moc_rev_required;
#elif MOC_REV != 67                                     // UndCC_Valid
#endif

/* Compliant - #ifdef opens a chain; the named identifier is guarded for
   subsequent branches of that chain. */
#ifdef GATED
#elif GATED == 0                                        // UndCC_Valid
#endif

/* Compliant - guard from the #if branch carries through to #elif of the
   same chain. */
#if defined(SHARED_TOK) && SHARED_TOK == 1
#elif SHARED_TOK == 2                                   // UndCC_Valid
#endif

/* Non-compliant - a sibling chain inside a parent chain does not
   inherit the parent's guards. */
#if defined(WRAPPER)
#if SIBLING_TOK == 0                                    // UndCC_Violation
#endif
#endif
