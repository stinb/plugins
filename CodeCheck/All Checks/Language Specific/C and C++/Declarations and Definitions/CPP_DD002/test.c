int var1 = 0;
#define MAC1 var1; //Invalid // UndCC_Violation
#define MAC2 5; //Valid
#define MAC3 var2;  //Invalid // UndCC_Violation
int var2 = 1;
