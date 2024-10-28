#include <cstddef>
#include <new>

void insert_in_table(int *table, std::size_t tableSize, std::size_t pos, int value)
{ // #1
    if (pos >= tableSize)
    {
        // Handle error
        return;
    }
    table[pos] = value;
}

template <std::size_t N>
void insert_in_table(int (&table)[N], std::size_t pos, int value)
{ // #2
    insert_in_table(table, N, pos, value);
}

void f()
{
    // Exposition only
    int table1[100];
    int *table2 = new int[100];
    insert_in_table(table1, 0, 0); // Calls #2
    // insert_in_table(table2, 0, 0);      // Error, no matching function call
    insert_in_table(table1, 100, 0, 0); // Calls #1
    insert_in_table(table2, 100, 0, 0); // Calls #1
    delete[] table2;
}