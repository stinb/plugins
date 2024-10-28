/*****************************************
* Function name: myFunc1
* Description: function description here
* Notes: Example of a valid prologue
* Arguments: int, int
* Return value: int
* Errors: you will get compiler error when...
* References: none
******************************************/
int myFunc1(int a, int b) { //UndCC_Valid
	int answer;
	answer = a+b;
	return answer;
}	


/*****************************************
* 
* Description: function description here
* Notes: Example of an invalid prologue (missing info, and missing error section completely)
* 
* Return value: int
* References: none
******************************************/
int myFunc2(int a, int b) { //UndCC_Violation
	int answer;
	answer = a-b;
	return answer;
}	
