// Custom

static int thisNameHas40CharactersAndTheLastCharIsA; // UndCC_Violation
static int thisNameHas40CharactersAndTheLastCharIsB; // UndCC_Violation

static int thisNameHas29CharsEndingWithA;
static int thisNameHas29CharsEndingWithB;

static int theNameHas31CharsAndLastCharIsB;
#define    theNameHas31CharsAndLastCharIsA

static int thisNameHas32CharsAndLastCharIsB; // UndCC_Violation
#define    thisNameHas32CharsAndLastCharIsA  // UndCC_Violation

static int thisNameHas48CharactersAndTheLastCharIsSomething; // UndCC_Violation
#define    thisNameHas48CharactersAndTheLastCharIsSomething  // UndCC_Violation
