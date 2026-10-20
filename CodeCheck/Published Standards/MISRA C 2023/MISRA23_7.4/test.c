#include <string.h>
#include "test.h"

typedef unsigned char uint8_t;
typedef char mychar_t;
typedef const char *cstring_t;

void takes_u8(uint8_t *s);
void takes_cvoid(const void *s);
void takes_two(int n, char *s);
int  takes_int(int a, int b);

uint8_t *gU8 = "global";                    // UndCC_Violation
cstring_t gCstr = "global";                 // UndCC_Valid - typedef of const char *

void stringLiterals(void)
{
    uint8_t  stringArray [20] = "CMRL ";    // UndCC_Valid - array copies the literal
    char     buf[] = "abc";                 // UndCC_Valid
    uint8_t *stringPtr = &stringArray[0];
    mychar_t *m;
    char *const cp = "x";                   // UndCC_Violation - const pointer to non-const char
    const char *const ccp = "x";            // UndCC_Valid
    const char *ok;
    char *a = "x", *b = "y";                // UndCC_Violation
    char *c = ("paren");                    // UndCC_Violation

    stringPtr = "A message";                // UndCC_Violation - uint8_t is unsigned char
    m = "typedef of char";                  // UndCC_Violation
    ok = "fine";                            // UndCC_Valid

    (void)memcpy(stringArray, "CMRL ", 5);  // UndCC_Violation - const void * is not const char *
    if(memcmp(stringArray, (uint8_t*)"ID3", 3) != 0){   // UndCC_Violation
        (void)strlen("fine");               // UndCC_Valid
    }

    takes_u8("u8");                         // UndCC_Violation
    takes_cvoid("cvoid");                   // UndCC_Violation
    takes_two(takes_int(1, 2), "nested");   // UndCC_Violation - comma inside a nested call
    (void)takes_int(1, 2);                  // UndCC_Valid

    (void)buf; (void)a; (void)b; (void)c; (void)cp; (void)ccp; (void)ok; (void)m;
}

uint8_t *name3(void)
{
    return "MISRA";                         // UndCC_Violation
}

const char *name4(void)
{
    return "MISRA";                         // UndCC_Valid
}

#define MSG       "message"
#define LOGNO(n)  "AH" #n ": "
#define NOT_STR   0

void macros(char *error, char c)
{
    char *p = MSG;                          // UndCC_Violation
    takes_two(1, LOGNO(01255) "text");      // UndCC_Violation
    takes_two(1, (char *)NOT_STR);          // UndCC_Valid
    c = 'x';                                // UndCC_Valid - character literal
    (void)p; (void)error; (void)c;
}

int vlog(char *tag, const char *fmt, ...)
{
    (void)tag; (void)fmt;
    return 0;
}

void variadic(char *t)
{
    (void)vlog(t, "format %s", "arg");      // UndCC_Valid - fmt is const char *, rest are variadic
    (void)vlog("tag", "format");            // UndCC_Violation
}

#define MARK          __FILE__, __LINE__
#define WRAP(word)    takes_u8((uint8_t *)0); (void)word
#define LOG(a, b, fmt, s)  takes_mark(a, b, s, fmt)
void takes_mark(const char *f, int l, char *s, const char *fmt);

void macroCalls(void)
{
    WRAP("word");                           // UndCC_Valid - argument belongs to the macro
    takes_mark(MARK, (char *)0, "fmt");     // UndCC_Valid - MARK expands to two arguments
    LOG(__FILE__, 1, "fmt", (char *)0);     // UndCC_Valid - macro reorders the arguments
}

void parameters(char *param, const char *cparam)
{
    param = "x";                            // UndCC_Violation
    cparam = "y";                           // UndCC_Valid
    (void)param; (void)cparam;
}
