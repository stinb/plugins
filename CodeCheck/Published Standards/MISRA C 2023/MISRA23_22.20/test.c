#if defined(__linux__) || defined(_WIN32)

#include <threads.h>
#include <stdint.h>

tss_t GlobalKey;
thrd_t id1, id2;

void local_key(void)
{
  tss_t key;
  tss_set(key, NULL);                /* UndCC_Violation(lin, win) - not yet created */
  tss_create(&key, NULL);            /* UndCC_Valid */
  tss_set(key, NULL);                /* UndCC_Valid */
  (void)tss_get(key);                /* UndCC_Valid */
  tss_delete(key);                   /* UndCC_Valid */
}

void reading_first(void)
{
  tss_t key;
  (void)tss_get(key);                /* UndCC_Violation(lin, win) - not yet created */
  tss_create(&key, NULL);            /* UndCC_Valid */
}

void wider_scope(tss_t param)
{
  (void)tss_get(GlobalKey);          /* UndCC_Valid - may be created elsewhere */
  (void)tss_get(param);              /* UndCC_Valid - a parameter says nothing */
}

void create_helper(tss_t *k) { tss_create(k, NULL); }
void use(void *p) { (void)p; }

void via_helper(void)
{
  tss_t key;
  create_helper(&key);               /* UndCC_Valid */
  (void)tss_get(key);                /* UndCC_Valid - the helper may have created it */
}

void escape_after_access(void)
{
  tss_t key;
  (void)tss_get(key);                /* UndCC_Violation(lin, win) - the later call cannot undo this */
  create_helper(&key);
}

void nested_access(void)
{
  tss_t key;
  use(tss_get(key));                 /* UndCC_Violation(lin, win) - a by-value use is not an escape */
  tss_create(&key, NULL);            /* UndCC_Valid */
}

/* The standard's example turns on thread ordering, a whole-program property */
tss_t key1;
tss_t key2;
int32_t g1;
int32_t g2;

int32_t t2(void *ignore)
{
  tss_create(&key1, NULL);           /* UndCC_FalseNeg - t1 might already have accessed key1 */
  return 0;
}

int32_t t1(void *ignore)
{
  tss_set(key1, &g1);                /* UndCC_FalseNeg - key1 might not yet be created */
  tss_set(key2, &g2);                /* UndCC_Valid */
  return 0;
}

void standard_example(void)
{
  tss_create(&key2, NULL);           /* UndCC_Valid */
  thrd_create(&id1, t1, NULL);
  thrd_create(&id2, t2, NULL);
}

#endif
