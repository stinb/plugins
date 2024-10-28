#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define RETIRE_AGE 65
const int ACTIVE = 0;

struct personal_info {

  char name[35]; /* Employee name */
  unsigned char age; /* Employee age */
  short status; /* Employee status - ACTIVE or NON_ACTIVE */

};

void process_data ( struct personal_info *emp_info ) /* Employee data */
{
  unsigned int emp_age; /* Employee age */
  short emp_status;
  emp_age = emp_info->age; /* Retrieve age as an unsigned int */ // UndCC_Violation
  emp_status = emp_info->status;

  if (emp_age >= RETIRE_AGE) {
    printf("%s is retired\n", emp_info->name);
  } /* endif */

} /* end of process_data */

void gen_employee_report (void)
{
  struct personal_info employee;
  strcpy(employee.name, "John Smith");
  employee.age = 37; /* Store age as an unsigned char */
  employee.status = ACTIVE;
  process_data(&employee);
} /* end of gen_employee_report */

union Data {
  int i;
  float f;
  char str[20];
};


