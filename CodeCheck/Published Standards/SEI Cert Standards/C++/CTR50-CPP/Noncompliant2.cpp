#include <vector>

void insert_in_table(std::vector<int> &table, long long pos, int value)
{
    if (pos >= table.size())
    {
        // Handle error
        return;
    }
    table[pos] = value; // UndCC_Violation
}
