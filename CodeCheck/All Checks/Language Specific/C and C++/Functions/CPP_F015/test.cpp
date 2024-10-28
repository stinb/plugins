#include "test.h"

void okay1() {}
void okay2(int p1) {}
void okay3(int p1,int p2,int p3) {}
void okay4(int (*p1)(),int (*p2)(int,int)) {}
void okay5(int p1,int) {}
void okay6(int,int p2) {}
void okay7(int p1,...) {}
void A::okay8(int p1) {}
void A::okay9(int p1,int p2,int p3) {}

void violation1(int p1) {}
void violation2(int p1) {}
void violation3(int p1,int p2,int p3) {}
void B::violation4(int p1) {}
void B::violation5(int p1,int p2,int p3) {}

// test ANSI decl, K&R defn
void okay10(p1,p2) int p1; int p2 {}

void okay11(int p1,char *p2);
void okay11(p1,p2) int p1; int p2 {}


typedef int Boolean;
typedef int Handle;
typedef int Memory;
Boolean 
InsertIndexEntry (Handle aHndl, 
                        long nodeEntry, 
                        Memory pData); 
Boolean 
InsertIndexEntry (aHndl, nodeEntry, pData) 
Handle aHndl; 
long nodeEntry; 
Memory pData; 
{ return (1); } 
