// Custom

#include <stdbool.h>
#include <stddef.h>

size_t a = sizeof(struct { int a : 1; });
size_t b = sizeof(struct { int : 1; });

struct { int c : 1; } c; // UndCC_Violation
struct { int : 1; } d;   // UndCC_Violation

typedef struct { int e : 1; } e; // UndCC_Violation
typedef struct { int : 1; } f;   // UndCC_Violation

struct g { int g : 1; }; // UndCC_Violation
struct h { int : 1; };   // UndCC_Violation

struct i { bool i : 1; };
struct j { bool : 1; };
