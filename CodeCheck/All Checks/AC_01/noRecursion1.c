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

void directRecursion() {
	directRecursion();          // UNDCC_Violation - Direct recursion
}

void indirectRecursion1_a() {
	indirectRecursion1_b();     // UNDCC_Violation - Indirect recursion
}

void indirectRecursion1_b() {
	indirectRecursion1_a();     // UNDCC_Violation - Indirect recursion
}

void indirectRecursion2_a() {
	indirectRecursion2_b();     // UNDCC_Violation - Indirect recursion
}

void indirectRecursion2_b() {
	indirectRecursion2_c();     // UNDCC_Violation - Indirect recursion
}

void indirectRecursion2_c() {
	if (0) {
		indirectRecursion2_a();    // UNDCC_Violation - Indirect recursion, even if it's potentially unreachable
	}
}
