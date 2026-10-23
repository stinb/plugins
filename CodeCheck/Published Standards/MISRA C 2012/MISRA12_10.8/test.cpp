namespace ns { typedef unsigned int u32; typedef unsigned long long u64; }

void qualifiedCast(ns::u32 a, ns::u32 b)
{
  ns::u64 w;
  w = (ns::u64)(a + b);                                // UndCC_Violation - wider, through a qualified name
  w = (ns::u64)a;                                      // UndCC_Valid
  int s = (int)(a + b);                                // UndCC_Violation - unsigned composite to signed
  (void)w; (void)s;
}
