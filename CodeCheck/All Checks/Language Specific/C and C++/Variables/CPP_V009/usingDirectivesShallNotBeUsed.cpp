namespace NS1
{
  int i1;
  int j1;
  int k1;
}
using namespace NS1; // UndCC_Violation

namespace NS2
{
  int i2;
  int j2;
  int k2;
}
using NS2::j2; // UndCC_Valid

namespace NS3
{
  int i3;
  int j3;
  int k3;
}

void f ()
{
  ++i1;
  ++j2;
  ++NS3::k3;
}
