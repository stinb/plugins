#include <stdatomic.h>
#include <stdbool.h>

static atomic_bool flag = ATOMIC_VAR_INIT(false);

void init_flag(void)
{
    atomic_init(&flag, false);
}

void toggle_flag(void)
{
    bool temp_flag = atomic_load(&flag);
    temp_flag = !temp_flag; // UNDCC_Violation, atomic variable referred twice
    atomic_store(&flag, temp_flag);
}

bool get_flag(void)
{
    return atomic_load(&flag);
}
