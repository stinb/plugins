// Custom

int need_more_privileges;
int escalated_privileges;

void fn_bad() { // UndCC_Violation
  /* Code intended to run with elevated privileges */

  /* Temporarily drop privileges */
  if (seteuid(getuid()) != 0) {
    /* Handle error */
  }

  /* Code intended to run with lower privileges */

  if (need_more_privileges) {
    /* Restore privileges */
    if (seteuid(escalated_privileges) != 0) {
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


void fn_good() {
  /* Code intended to run with elevated privileges   */

  /* Temporarily drop privileges */
  if (seteuid(getuid()) != 0) {
    /* Handle error */
  }

  /* Code intended to run with lower privileges */

  if (need_more_privileges) {
    /* Restore Privileges */
    if (seteuid(escalated_privileges) != 0) {
      /* Handle error */
    }

    /* Code intended to run with elevated privileges */
  }

  /* ... */

  /* Permanently drop privileges */
  if (setuid(getuid()) != 0) {
    /* Handle error */
  }

  if (setuid(escalated_privileges) == 0) {
    /* Privileges can be restored, handle error */
  }

  /*
  * Code intended to run with lower privileges;
  * attacker cannot regain elevated privileges
  */
}
