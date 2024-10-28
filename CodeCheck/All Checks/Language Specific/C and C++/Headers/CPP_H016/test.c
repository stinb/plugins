
/* Define a prototype */
int addNumbers(int a, int b); // UndCC_Violation

/* Define a typedef */
typedef int BYTE;

/* Define a macro */
#define KILOMETER 1000

/* Define a struct */
struct Car // UndCC_Violation
{
  int year;
  int miles;
  char make[20];
  char model[20];
};

/* Define an enum */
enum week{sunday, monday, tuesday, wednesday, thursday, friday, saturday}; // UndCC_Violation


void my_func() {
  
  /* Use all of the objects */
  BYTE b1 = 1;
  struct Car car;
  enum week day;
  day = wednesday;


  if (addNumbers(3,5) == 8) {
    printf("Correct!"); // UndCC_FalsePos - std lib function
  }
  printf("%d", b1);
  car.miles = KILOMETER;
  if (car.miles == 1000) {
    printf("%d\n", car.miles);
  }
  printf("%d\n",day); 

}
