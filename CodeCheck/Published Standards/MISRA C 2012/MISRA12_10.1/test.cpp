#include <stdbool.h>
#include <stdint.h>

enum enuma { a1, a2, a3 } ena, enb; /* Essentially enum<enuma> */
enum { K1 = 1, K2 = 2 };            /* Essentially signed (anonymous enum) */

struct Flags {
  unsigned int one  : 1;            /* Single-bit unsigned bit-field — essentially Boolean */
  unsigned int four : 4;            /* Wider bit-field — essentially Unsigned */
};

#define MASK_AND_VAR(x) ((x) & 0x1U)  /* Operator inside macro — suppressed */

int main(void) {
  float f32a;
  float f32b;

  char cha;
  int chb;
  signed int s8a;
  signed int s8b;
  signed int s16b;
  unsigned int u8a;
  unsigned int u8b;
  unsigned int u16b;
  bool bla = false;
  bool blb = false;

  unsigned int dev;
  unsigned int cfg;

  uint8_t n8a = 0;
  uint8_t n8b = 0;
  int16_t m16a = 0;
  int16_t m16b = 0;

  struct Flags fl = {0, 0};


  /* ------------------------------------------------------------------- */
  /* Existing MISRA examples — migrated from the Python check.           */
  /* ------------------------------------------------------------------- */

  int aa[2];
  int dummy_x = aa[bla];                // UndCC_Violation - Rationale 3 (bool as subscript)
  (void)(cha && bla);                   // UndCC_Violation - Rationale 2 (char as Boolean)
  (void)(ena ? a1 : a2);                // UndCC_Violation - Rationale 2 (enum as Boolean)
  (void)(s8a && bla);                   // UndCC_Violation - Rationale 2 (signed as Boolean)
  (void)(u8a ? a1 : a2);                // UndCC_Violation - Rationale 2 (unsigned as Boolean)
  (void)(f32a && bla);                  // UndCC_Violation - Rationale 2 (floating as Boolean)
  (void)(bla * blb);                    // UndCC_Violation - Rationale 3 (Boolean as numeric)
  (void)(bla > blb);                    // UndCC_Violation - Rationale 3 (Boolean as numeric)
  (void)(cha & chb);                    // UndCC_Violation - Rationale 4 (char as numeric)
  (void)(cha << 1);                     // UndCC_Violation - Rationale 4 (char as numeric)
  (void)(ena * a1);                     // UndCC_Violation - Rationale 5 (named enum in arithmetic)
  (void)(s8a & 2);                      // UndCC_Violation - Rationale 6 (bitwise on signed)
  (void)(50 << 3U);                     // UndCC_Violation - Rationale 6 (shift on signed literal)
  (void)(u8a << s8a);                   // UndCC_Violation - Rationale 7 (signed shift magnitude)
  (void)(u8a << -1);                    // UndCC_Violation - Rationale 7 (negative shift magnitude)
  (void)(-u8a);                         // UndCC_Violation - Rationale 8 (unary minus on unsigned)


  /* ------------------------------------------------------------------- */
  /* Issue #4897 — parenthesised non-Boolean operand of && / ||.          */
  /* ------------------------------------------------------------------- */

  if ((dev == 0u) && (cfg & 0x0002U)) { } // UndCC_Violation - RHS of && is essentially unsigned
  if ((dev == 0u) || (cfg & 0x0002U)) { } // UndCC_Violation - RHS of || is essentially unsigned
  if ((dev == 0u) && ((cfg & 0x0002U) != 0)) { }  // UndCC_Valid - compliant rewrite


  /* ------------------------------------------------------------------- */
  /* Anonymous-enum constants are essentially signed.                     */
  /* ------------------------------------------------------------------- */

  (void)(K1 * s8a);                     // UndCC_Valid - anonymous-enum constant is signed
  (void)(K1 && bla);                    // UndCC_Violation - anonymous-enum constant treated as signed in &&


  /* ------------------------------------------------------------------- */
  /* Single-bit unsigned bit-field is essentially Boolean.                */
  /* ------------------------------------------------------------------- */

  (void)(fl.one && bla);                // UndCC_Valid - 1-bit unsigned bit-field treated as Boolean
  (void)(fl.four && bla);               // UndCC_Violation - wider bit-field is essentially unsigned


  /* ------------------------------------------------------------------- */
  /* Macro suppression.                                                   */
  /* ------------------------------------------------------------------- */

  (void)MASK_AND_VAR(u8a);              // UndCC_Valid - & is inside macro, suppressed


  /* ------------------------------------------------------------------- */
  /* Narrow-type composite operands.  Integer promotion makes the Clang  */
  /* result type `int`, but MISRA's essential type model keeps the       */
  /* essential signedness of the operands.                               */
  /* ------------------------------------------------------------------- */

  (void)((n8a + n8b) << 2U);            // UndCC_Valid - unsigned composite stays essentially unsigned
  (void)((n8a & n8b) << 2U);            // UndCC_Valid - unsigned composite stays essentially unsigned
  (void)((m16a + m16b) << 2);           // UndCC_Violation - signed composite on shift left operand


  /* ------------------------------------------------------------------- */
  /* Valid cases — compliant per MISRA.                                   */
  /* ------------------------------------------------------------------- */

  (void)(bla && blb);                   // UndCC_Valid
  (void)(bla ? u8a : u8b);              // UndCC_Valid
  (void)(cha - chb);                    // UndCC_Valid
  (void)(cha > chb);                    // UndCC_Valid
  (void)(ena > a1);                     // UndCC_Valid
  (void)(s8a + s16b);                   // UndCC_Valid
  (void)(-(s8a) * s8b);                 // UndCC_Valid
  (void)(s8a > 0);                      // UndCC_Valid
  --s16b;                               // UndCC_Valid
  (void)(u8a + u16b);                   // UndCC_Valid
  (void)(u8a & 2U);                     // UndCC_Valid
  (void)(u8a > 0U);                     // UndCC_Valid
  (void)(u8a << 2U);                    // UndCC_Valid
  (void)(u8a << 1);                     // UndCC_Valid - Exception 1: non-negative ICE on shift RHS
  (void)(f32a + f32b);                  // UndCC_Valid
  (void)(f32a > 0.0);                   // UndCC_Valid

  return 0;
}
