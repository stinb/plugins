// SEI CERT C++

#include <condition_variable>
#include <mutex>

struct Node
{
    void *node;
    struct Node *next;
};

static Node list;
static std::mutex m;
static std::condition_variable condition;

void consume_list_element()
{
    std::unique_lock<std::mutex> lk(m);

    condition.wait(lk, []
                   { return list.next; });
    // Proceed when condition holds.
}
