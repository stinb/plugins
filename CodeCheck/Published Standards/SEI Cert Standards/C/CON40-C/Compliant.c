#include <stdatomic.h>
#include <stdbool.h>

static atomic_bool flag = ATOMIC_VAR_INIT(false);

void init_flag(void)
{
    atomic_init(&flag, false);
}

void toggle_flag(void)
{
    bool old_flag = atomic_load(&flag);
    bool new_flag;
    do
    {
        new_flag = !old_flag;
    } while (!atomic_compare_exchange_weak(&flag, &old_flag, new_flag));
}

bool get_flag(void)
{
    return atomic_load(&flag);
}

int compute_sum(int n)
{
    return n * (n + 1) / 2;
}