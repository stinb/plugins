#include <vector>

void insert_in_table(std::vector<int> &table, std::size_t pos, int value)
{
    if (pos >= table.size())
    {
        // Handle error
        return;
    }
    table[pos] = value;
}