// SEI CERT C

int need_more_privileges;

void fn() {
  /* Code intended to run with elevated privileges   */

  /* Temporarily drop privileges */
  if (seteuid(getuid()) != 0) {
    /* Handle error */
  }

  /* Code intended to run with lower privileges */

  if (need_more_privileges) {
    /* Restore Privileges */
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

  if (setuid(0) != -1) {
    /* Privileges can be restored, handle error */
  }

  /*
  * Code intended to run with lower privileges;
  * attacker cannot regain elevated privileges
  */
}
