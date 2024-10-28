/* f prototype in scope in this source file */

long f(long x);

long g(int x)
{
    return f((long)x);
}
