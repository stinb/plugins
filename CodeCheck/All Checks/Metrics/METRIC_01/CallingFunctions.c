void invalidCallers(){ // UndCC_Violation
  ;
}

void validCaller1(){
  invalidCallers();
}

void validCaller2(){
  invalidCallers();
}

void validCaller3(){
  invalidCallers();
}

void validCaller4(){
  invalidCallers();
}

void validCaller5(){
  invalidCallers();
}

void validCaller6(){
  validCaller1();
  invalidCallers();
}

