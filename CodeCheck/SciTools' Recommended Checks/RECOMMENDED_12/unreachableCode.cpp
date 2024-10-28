void valid(){
	int i=1;
}


void unreachableinvalid1(){
	int i=1;
	return i;
	i=5; // UndCC_Violation
	int j=2;
}
