#include <vector>

void insert_in_table(std::vector<int> &table, std::size_t pos, int value) noexcept(false)
{
    table.at(pos) = value;
}