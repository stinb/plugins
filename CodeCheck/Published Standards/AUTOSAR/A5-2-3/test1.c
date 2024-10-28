// MISRA C 2012

#include <stdint.h>


void f()
{
           uint16_t          x;
           uint16_t * const  cpi = &x;   /* const pointer               */
           uint16_t * const *pcpi;       /* pointer to const pointer    */
           uint16_t *       *ppi;
     const uint16_t         *pci;        /* pointer to const            */
  volatile uint16_t         *pvi;        /* pointer to volatile         */
           uint16_t         *pi;

  pi = cpi;                              /* UndCC_Valid - no conversion
                                                     no cast required   */

  pi  = (uint16_t *)pci;                 /* UndCC_Violation             */
  pi  = (uint16_t *)pvi;                 /* UndCC_Violation             */
  ppi = (uint16_t * *)pcpi;              /* UndCC_Violation             */
}
