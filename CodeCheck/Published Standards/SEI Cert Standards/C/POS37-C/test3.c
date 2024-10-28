// SEI CERT C

typedef int gid_t;

/* Returns nonzero if the two group lists are equivalent (taking into
  account that the lists may differ wrt the egid */
int eql_sups(const int cursups_size, const gid_t* const cursups_list,
        const int targetsups_size, const gid_t* const targetsups_list) {
  int i;
  int j;
  const int n = targetsups_size;
  const int diff = cursups_size - targetsups_size;
  const gid_t egid = getegid();
  if (diff > 1 || diff < 0 ) {
    return 0;
  }
  for (i=0, j=0; i < n; i++, j++) {
    if (cursups_list[j] != targetsups_list[i]) {
      if (cursups_list[j] == egid) {
    i--; /* skipping j */
      } else {
    return 0;
      }
    }
  }
  /* If reached here, we're sure i==targetsups_size. Now, either
    j==cursups_size (skipped the egid or it wasn't there), or we didn't
    get to the egid yet because it's the last entry in cursups */
  return j == cursups_size ||
    (j+1 == cursups_size && cursups_list[j] == egid);
}
