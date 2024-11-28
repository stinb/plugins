#include <stdint.h>

const uint16_t cf(void);   /* UndCC_Valid - returns const uint16_t */
const uint16_t *pcf(void); /* UndCC_Valid - returns a pointer to const uint16_t */

typedef uint16_t ftype(void);
typedef const ftype cftype;            /* UndCC_Violation - cftype is const-qualified */
typedef ftype dftype;                  /* UndCC_Valid - dftype is not qualified */
typedef ftype *const pcftype;          /* UndCC_Valid - const pointer to ftype */
typedef const uint16_t *cfptype(void); /* UndCC_Valid - cfptype is the type of a function returning const uint16_t * */