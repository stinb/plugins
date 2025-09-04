#define UINT_32 unsigned int

int main() {
  UINT_32 uLen = 123456789;
  UINT_32 m_count[2] = {0};
  m_count[1] += (uLen >> 29); // UndCC_Violation(1)

}
