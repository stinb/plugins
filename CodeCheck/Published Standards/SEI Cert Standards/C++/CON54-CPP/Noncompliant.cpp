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

void consume_list_element(std::condition_variable &condition)
{
    std::unique_lock<std::mutex> lk(m);

    if (list.next == nullptr)
    {
        condition.wait(lk);     // UndCC_Violation
    }

    // Proceed when condition holds.
}
