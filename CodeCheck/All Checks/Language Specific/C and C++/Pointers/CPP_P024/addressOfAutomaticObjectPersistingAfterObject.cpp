#include <cstdint>


// From MISRA04_17.6
int8_t * barfoo( void )
{
    int8_t local_auto;
    return (&local_auto); // UNDCC_Violation and compiler warning
}


// From MISRA08_7-5-2
void foobar ( void )
{
    int8_t * p1;
    {
        int8_t local_auto;
        p1 = &local_auto; // UNDCC_Violation
    }
}


// From MISRA12_18.6
uint16_t *sp;
void g ( uint16_t *p )
{
    sp = p;               // UNDCC_Violation - address of g's parameter u copied to static sp
}
void f ( uint16_t u )
{
    g ( &u );
}
void h ( void )
{
    static uint16_t *q;
    uint16_t x = 0u;
    q = &x;               // UNDCC_Violation - &x stored in object with greater lifetime
}


// Additional tests
static int8_t *stat_obj;

int8_t *i( void )
{
    int8_t *p1;
    {
        int8_t local_auto;
        p1 = &local_auto;            // UNDCC_Violation
        stat_obj = &local_auto;      // UNDCC_Violation
        return &local_auto;          // UNDCC_Violation and compiler warning
        {
            int8_t *ptr_local_auto2;
            p1 = ptr_local_auto2;    // UNDCC_Violation
        }
    }
}

char *a;
void j( void )
{
    char c = 'a';
    a[0] = c;                        // UNDCC_Valid
}
