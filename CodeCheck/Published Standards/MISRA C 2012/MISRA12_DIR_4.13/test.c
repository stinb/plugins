#include <stdio.h>

typedef int mutex_t;
extern mutex_t mutex_lock(void);
extern void mutex_unlock(mutex_t m);

extern void use_file(FILE *f);
extern int condition(void);

FILE *g_fp;

/* The MISRA example: locked, but not unlocked on the else path. */
void mutex_leak(int x)
{
  mutex_t m = mutex_lock();  // UndCC_Violation

  if (x > 0)
  {
    mutex_unlock(m);
  }
  else
  {
    /* Mutex not unlocked on this path */
  }
}

/* fopen on a branch, fclose only on that branch's success leg. */
void file_leak_one_branch(int x)
{
  FILE *fp = fopen("a", "r");  // UndCC_Violation

  if (x > 0)
  {
    fclose(fp);
  }
}

/* Opened, used, but never closed on any path. The deallocator never appears, so
   the resource is conservatively assumed to be owned elsewhere and is not
   reported (passing it to use_file is an operation, not ownership transfer). */
void never_closed(void)
{
  FILE *fp = fopen("a", "r");  // UndCC_Valid
  use_file(fp);
}

/* Closed on every fall-through path. */
void closed_both_branches(int x)
{
  FILE *fp = fopen("a", "r");  // UndCC_Valid

  if (x > 0)
  {
    fclose(fp);
  }
  else
  {
    fclose(fp);
  }
}

/* Closed unconditionally after a branch. */
void closed_after_branch(int x)
{
  FILE *fp = fopen("a", "r");  // UndCC_Valid

  if (x > 0)
  {
    use_file(fp);
  }

  fclose(fp);
}

/* Common error-handling pattern: early return when the open failed. */
void early_return_on_null(void)
{
  FILE *fp = fopen("a", "r");  // UndCC_Valid

  if (fp == NULL)
  {
    return;
  }

  fclose(fp);
}

/* mutex pattern with both legs unlocking. */
void mutex_balanced(int x)
{
  mutex_t m = mutex_lock();  // UndCC_Valid

  if (x > 0)
  {
    use_file(0);
  }

  mutex_unlock(m);
}

/* Ownership transfer: the resource is returned to the caller. */
FILE *returns_resource(void)
{
  FILE *fp = fopen("a", "r");  // UndCC_Valid
  return fp;
}

/* Ownership transfer: stored in a global. */
void stores_in_global(void)
{
  FILE *fp = fopen("a", "r");  // UndCC_Valid
  g_fp = fp;
}

/* Ownership transfer: handed to another function. */
void passes_to_helper(void)
{
  FILE *fp = fopen("a", "r");  // UndCC_Valid
  use_file(fp);
  /* helper is assumed to take ownership */
}

/* Closed inside a loop body that always runs is not assumed; closing on the
   single fall-through path after the loop is the compliant shape here. */
void closed_after_loop(int n)
{
  FILE *fp = fopen("a", "r");  // UndCC_Valid
  int i;

  for (i = 0; i < n; i++)
  {
    use_file(fp);
  }

  fclose(fp);
}

/* Not a tracked resource: no allocator pairing, so nothing to report. */
void untracked_local(void)
{
  int x = condition();  // UndCC_Valid
  use_file(0);
}

/* Closed on one path, ownership transferred out (returned) on the other. The
   deallocator is called, so this exercises the escape suppression rather than
   the never-released path. */
FILE *close_or_return(int x)
{
  FILE *fp = fopen("a", "r");  // UndCC_Valid

  if (x > 0)
  {
    fclose(fp);
    return 0;
  }

  return fp;
}

/* goto-based cleanup: the error leg jumps to the single release at the end. */
void goto_cleanup(int x)
{
  FILE *fp = fopen("a", "r");  // UndCC_Valid

  if (x > 0)
  {
    goto done;
  }

  use_file(fp);

done:
  fclose(fp);
}

/* break out of a loop, then the single fall-through release. */
void break_in_loop(int n)
{
  FILE *fp = fopen("a", "r");  // UndCC_Valid
  int i;

  for (i = 0; i < n; i++)
  {
    if (i == 5)
    {
      break;
    }
    use_file(fp);
  }

  fclose(fp);
}

extern FILE *popen(const char *command, const char *mode);
extern int pclose(FILE *stream);

/* A non-file default pair (popen/pclose), closed only on one branch. */
void popen_leak(int x)
{
  FILE *p = popen("ls", "r");  // UndCC_Violation

  if (x > 0)
  {
    pclose(p);
  }
}

/* Released inside a null-check of the resource: the branch that skips fclose is
   the path on which fopen failed and fp is NULL. */
void closed_when_non_null(void)
{
  FILE *fp = fopen("a", "r");  // UndCC_Valid

  if (fp)
  {
    use_file(fp);
    fclose(fp);
  }
}

typedef struct DIR DIR;
extern DIR *opendir(const char *name);
extern int closedir(DIR *dirp);
extern void use_dir(DIR *d);

/* Allocation tested in the controlling condition (opendir/closedir default
   pair); the else leg runs only when opendir returned NULL. */
void opendir_or_fallback(void)
{
  DIR *d;

  if ((d = opendir("/tmp")))  // UndCC_Valid
  {
    use_dir(d);
    closedir(d);
  }
  else
  {
    use_file(0);
  }
}
