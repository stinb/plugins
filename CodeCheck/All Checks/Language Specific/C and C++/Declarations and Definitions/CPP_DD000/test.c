#include <stdio.h>
#define NAME "Name"

typedef unsigned int uint;
typedef unsigned long ulong;

extern int a;
extern int b;

int c;
int d;

static int e;
static int f;

extern int func(void);
extern int func0(void);

int func1(void);
int func2(void);

struct student 
{
  int id;
  char name[20];
  float percentage;
};

int main()
{
}


#define NUMBER 3 // UNDCC_Violation
typedef int* IntPtr; // UNDCC_Violation
extern int g; // UNDCC_Violation
int h; // UNDCC_Violation
static int i; // UNDCC_Violation
extern int func3(void); // UNDCC_Violation
int func4(void); // UNDCC_Violation


struct student1
{ // UNDCC_Violation
  int id1;
  char name1[20];
  float percentage1;
};
