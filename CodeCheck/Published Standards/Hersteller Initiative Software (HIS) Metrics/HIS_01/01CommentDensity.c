

void validCommentDensity1(){
  int a;  //comment 1
  int b;  /* commment 2 */
}

/*Preceeding comment
that spans
multiple lines */
void validCommentDensity2(){
  int a;
  int b;
}


void invalidCommentDensity1(){ // UndCC_Violation
  int a;
  int b;
  int c;
  int d;
}

void invalidCommentDensity2(){ // UndCC_Violation
  int a; //1 comment
  int b;
  int c;
  int d;
  int e;
  int f;
  int g;
  int h;
  int i;
}
