// SEI CERT C

int need_more_privileges;

void fn() { // UndCC_Violation
  /* Code intended to run with elevated privileges */

  /* Temporarily drop privileges */
  if (seteuid(getuid()) != 0) {
    /* Handle error */
  }

  /* Code intended to run with lower privileges */

  if (need_more_privileges) {
    /* Restore privileges */
    if (seteuid(0) != 0) {
      /* Handle error */
    }

    /* Code intended to run with elevated privileges */
  }

  /* ... */

  /* Permanently drop privileges */
  if (setuid(getuid()) != 0) {
    /* Handle error */
  }

  /*
  * Code intended to run with lower privileges,
  * but if privilege relinquishment failed,
  * attacker can regain elevated privileges!
  */
}
