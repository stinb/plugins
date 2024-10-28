int foo()
{
  float*num;
  int*i;
  num = i; //UndCC_Violation
}

float bar(int* ptr){
   float num = 7.4; //UndCC_Valid
   return num; 
  
}

char letter(char* character)
{
  int*j;
  character = j; //UndCC_Violation
}
