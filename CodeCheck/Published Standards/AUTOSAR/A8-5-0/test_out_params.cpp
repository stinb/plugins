extern "C" int sscanf(const char *s, const char *format, ...);
void fill(char *buf, unsigned long n);
void fillLen(unsigned char *buf, int *len);

void newArrayCompared(int n)
{
    char *source = new char[n];
    if (source == 0)          // UndCC_Valid - the pointer itself is set by new
        return;
    source[n - 1] = '\0';
    delete[] source;
}

void newArrayCondition(int n)
{
    char *list = new char[n];
    if (list)                 // UndCC_Valid
    {
        list[0] = '\0';
        delete[] list;
    }
}

void newArrayPassed(int n)
{
    char *buffer = new char[n];
    fill(buffer, n);          // UndCC_Valid
    delete[] buffer;
}

void newArrayElementRead(int n)
{
    char *p = new char[n];
    char c = p[0];            // UndCC_Violation
    delete[] p;
}

void newArrayCheckedThenRead(int n)
{
    char *p = new char[n];
    if (p)
    {
        char c = p[0];        // UndCC_Violation
    }
    delete[] p;
}

void newScalarRead()
{
    int *p = new int;
    int k = *p;               // UndCC_Violation
    delete p;
}

void arrayFilledBySscanf(const char *in)
{
    unsigned major, minor;
    char code[4];
    if (sscanf(in, "%u.%u %3s", &major, &minor, code) == 3) // UndCC_Valid
        fill(code, 4);
}

void arraysFilledBySscanf(const char *in)
{
    char type[5], lint[2];
    int id;
    if (sscanf(in, "%4s %d %1s", type, &id, lint) == 3) // UndCC_Valid
        fill(type, 5);
}

void arrayFilledByCall()
{
    char parsed[16];
    fill(parsed, sizeof(parsed)); // UndCC_Valid
    fill(parsed, 16);
}

void arrayFilledWithLength()
{
    unsigned char val[8];
    int len;
    fillLen(val, &len);       // UndCC_Valid
    val[len] = 0;
}

void arrayElementRead()
{
    char arr[4];
    char c = arr[0];          // UndCC_Violation
}

struct Range { char *text; };
long send(int msg, long w, long l);
bool getPair(int &a, int &b);
int sum(const int &a, const int &b);
void sortLines();

void filledByReference()
{
    int w, h;
    if (getPair(w, h))        // UndCC_Valid
        w = w + h;
}

void readByConstReference()
{
    int a, b;
    int s = sum(a, b);        // UndCC_Violation
}

void catchParameters()
{
    try
    {
        sortLines();
    }
    catch (unsigned long &idx)
    {
        unsigned long n = idx + 1; // UndCC_Valid
    }
    catch (int i)
    {
        int k = i;            // UndCC_Valid
    }
}

void arrayAddressStored()
{
    char eol[3];
    Range tr;
    tr.text = eol;
    send(1, 0, reinterpret_cast<long>(&tr));
    if (eol[0] == '\r')       // UndCC_Valid
    {
    }
}
