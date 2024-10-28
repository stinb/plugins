void notRecursive_b();
void directRecursion();
void indirectRecursion1_b();
void indirectRecursion2_b();
void indirectRecursion2_c();

void notRecursive_a() {
  notRecursive_b();
}

void notRecursive_b() {
  directRecursion();
  // Even though it calls some other recursive function here, neither of these
  // two are considered "unsafe" - the function below is the one that's a
  // violation.
}

void directRecursion() {
  directRecursion(); // UndCC_Violation - obviously "unsafe"
}

void indirectRecursion1_a() {
  indirectRecursion1_b(); // UndCC_Violation
}

void indirectRecursion1_b() {
  indirectRecursion1_a(); // UndCC_Violation
}

void indirectRecursion2_a() {
  indirectRecursion2_b(); // UndCC_Violation
}

void indirectRecursion2_b() {
  indirectRecursion2_c(); // UndCC_Violation
}

void indirectRecursion2_c() {
  if (0) {
    indirectRecursion2_a(); // UndCC_Violation
    // Even if it's potentially unreachable, recursion is considered "unsafe."
  }
}
