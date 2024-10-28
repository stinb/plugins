extern int fn654();
int n654 = 1;
int x654;

class T {
public:
	int start();
	int end();
};

T thing654 = T();


int main () {
	for ( int x654 = 0; x654 < 10; ++x654 ) { // UndCC_Valid 
	}
	
	int y654;
	for ( int x654 = 0; x654 < 10; ++x654 ) {  
		y654 = 1;
		x654 = 0;
		int z654 = 1;
	}
	
	for ( x654 = thing654.start( ); x654 != thing654.end( ); ++x654 ) { // UndCC_Valid
	}
	
	for ( x654 = thing654.start( ); x654 != thing654.end( ); x654 = 2 ) { // UndCC_Violation
	}
	
	for ( x654 = 0; x654 < 10; x654 += 1 ) { // UndCC_Valid
	}
	
	for ( x654 = 0; x654 < 10; x654 += n654 ) { // UndCC_Valid if n is not modified
	}                                           // within the body of the loop.
	
	for ( x654 = 0; x654 < 10; x654 += n654 ) { // UndCC_Violation
		n654++;
	}                                           
	
	for ( x654 = 0; x654 < 10; x654 += fn654 ( ) )  { // UndCC_Violation
	}
						
}

