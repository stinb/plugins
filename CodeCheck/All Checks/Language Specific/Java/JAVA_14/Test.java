interface CompliantI {     // UNDCC_Valid
}

class CompliantC {         // UNDCC_Valid
}

interface nonCompliantI {  // UNDCC_Violation
}

class nonCompliantC {      // UNDCC_Violation
}

interface FalsenegativeI { // UNDCC_FalseNeg
}

class FalsenegativeC {     // UNDCC_FalseNeg
}
