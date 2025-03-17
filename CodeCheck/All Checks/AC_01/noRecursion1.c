void notRecursive_a();
void notRecursive_b();
void directRecursion();
void indirectRecursion1_a();
void indirectRecursion1_b();
void indirectRecursion2_a();
void indirectRecursion2_b();
void indirectRecursion2_c();

void notRecursive_a() {
	notRecursive_b();
}

void notRecursive_b() {
	directRecursion();
}

void directRecursion() { // UNDCC_Violation - Direct recursion
	directRecursion();
}

void indirectRecursion1_a() { // UNDCC_Violation - Indirect recursion
	indirectRecursion1_b();
}

void indirectRecursion1_b() { // UNDCC_Violation - Indirect recursion
	indirectRecursion1_a();
}

void indirectRecursion2_a() { // UNDCC_Violation - Indirect recursion
	indirectRecursion2_b();
}

void indirectRecursion2_b() { // UNDCC_Violation - Indirect recursion
	indirectRecursion2_c();
}

void indirectRecursion2_c() { // UNDCC_Violation - Indirect recursion, even if it's potentially unreachable
	if (0) {
		indirectRecursion2_a();
	}
}
