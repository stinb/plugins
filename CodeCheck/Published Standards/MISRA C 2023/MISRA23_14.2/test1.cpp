int main() {
  bool flag = false;
  for (int i = 0; ( i < 5 ) && !flag; i++ ) // UndCC_Violation
  {
    bool C = true;
    if ( C )
    {
      flag = true; /* Compliant but for loop as a whole flagged due to line 11 - allows early termination
                    * of loop */
    }
    i = i + 3; /* Non-compliant flagged above - altering the loop
                * counter */
  }
}
