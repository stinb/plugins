#if defined(__linux__) || defined(_WIN32)

#include <threads.h>
#include <stdint.h>

mtx_t Global;
cnd_t GlobalCnd;

void local_mutex(void)
{
  mtx_t m;
  mtx_lock(&m);                      /* UndCC_Violation(lin, win) - not yet initialized */
  mtx_init(&m, mtx_plain);           /* UndCC_Valid */
  mtx_lock(&m);                      /* UndCC_Valid */
  mtx_unlock(&m);                    /* UndCC_Valid */
  mtx_destroy(&m);                   /* UndCC_Valid */
}

void local_cnd(void)
{
  cnd_t c;
  mtx_t m;
  mtx_init(&m, mtx_timed);           /* UndCC_Valid */
  cnd_signal(&c);                    /* UndCC_Violation(lin, win) - not yet initialized */
  cnd_init(&c);                      /* UndCC_Valid */
  cnd_broadcast(&c);                 /* UndCC_Valid */
  cnd_wait(&c, &m);                  /* UndCC_Valid */
}

void wider_scope(mtx_t *param)
{
  mtx_lock(&Global);                 /* UndCC_Valid - may be initialized elsewhere */
  cnd_signal(&GlobalCnd);            /* UndCC_Valid - may be initialized elsewhere */
  mtx_lock(param);                   /* UndCC_Valid - a parameter says nothing */
}

void mutex_types(void)
{
  mtx_t a, b, c, d, e, f;
  mtx_init(&a, mtx_plain);                   /* UndCC_Valid */
  mtx_init(&b, mtx_timed);                   /* UndCC_Valid */
  mtx_init(&c, mtx_plain | mtx_recursive);   /* UndCC_Valid */
  mtx_init(&d, mtx_timed | mtx_recursive);   /* UndCC_Valid */
  mtx_init(&e, mtx_recursive);               /* UndCC_Violation(lin, win) - not a valid type */
  mtx_init(&f, 0);                           /* UndCC_Violation(lin, win) - not a valid type */
}

void init_helper(mtx_t *m) { mtx_init(m, mtx_plain); }

void via_helper(void)
{
  mtx_t m;
  init_helper(&m);                   /* UndCC_Valid */
  mtx_lock(&m);                      /* UndCC_Valid - the helper may have initialized it */
}

#define MY_KIND mtx_plain

void unknowable_type(int32_t kind)
{
  mtx_t m, n;
  mtx_init(&m, kind);                /* UndCC_Valid - a variable may hold a permitted value */
  mtx_init(&n, MY_KIND);             /* UndCC_Valid - a macro may expand to a permitted value */
}

void array_of_mutexes(void)
{
  mtx_t locks[2];
  mtx_lock(&locks[0]);               /* UndCC_FalseNeg - an array element is not resolved */
}

/* The standard's own example turns on thread ordering, which is a property of
   the whole program */
mtx_t Ra;
mtx_t Rb;
mtx_t Rc;
thrd_t id1, id2;

int32_t t1(void *ignore)
{
  mtx_init(&Rb, mtx_plain);          /* UndCC_FalseNeg - T2 may have already accessed Rb */
  mtx_lock(&Ra);
  mtx_unlock(&Ra);
  return 0;
}

int32_t t2(void *ignore)
{
  mtx_lock(&Rc);
  mtx_unlock(&Rc);
  return 0;
}

void standard_example(void)
{
  mtx_init(&Ra, mtx_plain);          /* UndCC_Valid */
  thrd_create(&id1, t1, NULL);
  thrd_create(&id2, t2, NULL);
  mtx_init(&Rc, mtx_plain);          /* UndCC_FalseNeg - the threads may have accessed Rc */
  thrd_join(id1, NULL);
  thrd_join(id2, NULL);
}

void escape_after_access(void)
{
  mtx_t m;
  mtx_lock(&m);                      /* UndCC_Violation(lin, win) - the later call cannot undo this */
  init_helper(&m);
}

void invalid_type_on_global(void)
{
  mtx_init(&Ra, mtx_recursive);      /* UndCC_Violation(lin, win) - reported whatever the scope */
}

struct Holder { mtx_t lock; };

void struct_member(void)
{
  struct Holder h;
  mtx_lock(&h.lock);                 /* UndCC_FalseNeg - a member is not resolved */
}

void parenthesised_type(void)
{
  mtx_t m;
  mtx_init(&m, (mtx_plain | mtx_recursive));   /* UndCC_Valid */
}

#endif
