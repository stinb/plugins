// Custom

#define STATIC static

static void f ( void );
STATIC void f ( void ) { }

static void g ( void );
       void g ( void ) { } // UndCC_Violation

static union h {} hObject;
extern union h hObject; // UndCC_Violation

static struct i {} iObject;
extern struct i iObject; // UndCC_Violation

static enum j {J} jObject;
extern enum j jObject; // UndCC_Violation
