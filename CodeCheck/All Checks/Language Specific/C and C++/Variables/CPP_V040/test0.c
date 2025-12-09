// MISRA C 2023

#ifdef _WIN32 // C11 threading not supported on Mac or Linux
#include <stdint.h>
#include <threads.h>


mtx_t Ra;
mtx_t Rb;
cnd_t Cnd;


int32_t t1(void *ignore )
{
  mtx_lock  ( &Ra       );
  cnd_wait  ( &Cnd, &Ra );    /* UndCC_Violation(Win) - t2 uses Cnd with Rb */
  mtx_unlock( &Ra       );
  return 0;
}


int32_t t2(void *ignore )
{
 mtx_lock  ( &Rb       );
 cnd_wait  ( &Cnd, &Rb );     /* UndCC_Violation(Win) - t1 uses Cnd with Ra */
 mtx_unlock( &Rb       );
 return 0;
}


int32_t t3(void* ignore)
{
 cnd_signal( &Cnd );          /* Unblocks one of Ra and Rb...
                                 ... unclear whether t1 or t2 resumes */
 return 0;
}
#endif
