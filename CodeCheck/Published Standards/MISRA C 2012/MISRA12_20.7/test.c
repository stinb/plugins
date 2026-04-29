/* Examples taken from MISRA C 2012 Rule 20.7 */

/* Non-compliant: parameters not enclosed in parentheses */
#define M1( x, y ) ( x * y )                            // UndCC_Violation

/* Compliant: each parameter is parenthesized */
#define M2( x, y ) ( ( x ) * ( y ) )                    // UndCC_Valid

/* Compliant: first x is operand of ##, second x is parenthesized */
#define M3( x ) a ## x = ( x )                          // UndCC_Valid

/* Compliant: M used as member name (no expression), S parenthesized */
#define GET_MEMBER( S, M ) ( S ).M                      // UndCC_Valid

/* Compliant: chained macro - F passes X as G's argument, G parenthesizes */
#define F( X ) G( X )                                   // UndCC_Valid
#define G( Y ) ( ( Y ) + 1 )                            // UndCC_Valid

/* Stringification: parameter not flagged */
#define STR( X ) #X                                     // UndCC_Valid

/* Token concatenation: parameter not flagged */
#define CONCAT( A, B ) A ## B                           // UndCC_Valid

/* Bare parameter in body: must be parenthesized */
#define BARE( X ) X                                     // UndCC_Violation

/* Unary operator on parameter: not parenthesized */
#define NEG( X ) -X                                     // UndCC_Violation

/* Parameter as function/macro call argument: enclosed by call's parens */
#define CALL( X ) f( X )                                // UndCC_Valid
#define CALL2( X, Y ) f( X, Y )                         // UndCC_Valid

/* Parameter combined with operator inside a function call: still flagged */
#define ADD_ONE( X ) f( X + 1 )                         // UndCC_Violation

/* Member access on parameter without parens: flagged */
#define BAD_MEMBER( S, M ) S.M                          // UndCC_Violation

/* Address-of and dereference: parameter not parenthesized */
#define ADDR( X ) &X                                    // UndCC_Violation
#define DEREF( X ) *X                                   // UndCC_Violation

/* Array indexing acts as a safe delimiter; A is unparenthesized */
#define AT( A, I ) A[ I ]                               // UndCC_Violation

/* Compound: array index plus arithmetic; both A and I problematic */
#define BAD_AT( A, I ) A[ I + 1 ]                       // UndCC_Violation

/* Multiple uses of same parameter: each occurrence checked */
#define SQ( X ) ( X * X )                               // UndCC_Violation

/* No parameters: rule does not apply */
#define NOP( ) do { } while ( 0 )                       // UndCC_Valid

/* Parameter used twice, both compliant */
#define ABS( X ) ( ( ( X ) < 0 ) ? -( X ) : ( X ) )     // UndCC_Valid

/* Multi-line macro via line continuation: parameter occurrences across
   continued lines must still be checked, but the walk must stop at the
   real newline ending the directive. The body starts on the continued
   line so the violation annotation can be placed at end-of-line. */
#define MULTILINE( r )                                  \
    ( !r->main || ( r->main->uri && r->uri ) )          // UndCC_Violation

/* Code outside the macro: identifier `r` here must NOT be flagged as a
   spurious occurrence of the MULTILINE parameter (regression test for
   the loop walking past the end of the macro body). */
struct dummy_ctx { int *r; };                           // UndCC_Valid
void use_r(int *r) { (void)r; }                         // UndCC_Valid

/* Parameter at the start of a continuation line, with the previous
   significant token ('(' from f() on the previous line) reachable only by
   crossing the line continuation. Should NOT be flagged. */
#define CALL_CONT( X ) f(                               \
    X )
