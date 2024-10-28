
int main() {
	/* good escapes */
	char * st = "\a \b \f \n \r \t \v \' \" \?  \\";
	char end = '\0';
	/* bad escapes */
	char * x = "\012 \G"; // UndCC_Violation
	char * y = "\xff";    // UndCC_Violation
	// end = '\N';        // Analysis error
	return 1;
}
