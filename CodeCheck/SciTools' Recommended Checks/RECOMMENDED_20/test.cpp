#include <iostream>

int count1;
int count2;
int count3;
int count4;

bool signal1;
bool signal2;
bool signal3;
bool signal4;

typedef struct {
  int                   threshold;
  int*                  count;
  bool*                 signal;
} LabelTbl_T;


// While the rest of the code is also duplicate, below is only where it meets
// the default option minimum requirements
const LabelTbl_T  chan_1_tbl[] = // UndCC_Violation
{
  //threshold, count, signal
  {12, &count1,  &signal1},
  {14, &count2,  &signal2},
  {14, &count3,  &signal3},
  {14, &count4,  &signal4},
};
