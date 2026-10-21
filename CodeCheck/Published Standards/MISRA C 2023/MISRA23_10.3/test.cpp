typedef unsigned char uint8_t;
typedef signed char int8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
int main() {
  enum enuma { A1, A2, A3 } ena;
  enum enumb { B1, B2, B3 } enb;
  enum { K1=1, K2=128 };
  uint8_t u8a = 0; /* By exception */
  int8_t s8a = 0;
  uint8_t * pu8a;
  uint8_t* pu8b;
  uint8_t u8b = 0;
  uint8_t u8c = 0;
  uint8_t u8d = 0;
  uint16_t u16a;
  uint32_t u32a;
  bool flag = ( bool ) 0;
  bool set = true; /* true is essentially Boolean */
  bool get = ( u8b > u8c );
  char cha = 'a';
  ena = A1;
  s8a = K1; /* Constant value fits */
  u8a = 2; /* By exception */
  u8a = 2 * 24; /* By exception */
  cha += 1; /* cha = cha + 1 assigns character to character */
  pu8a = pu8b; /* Same essential type */
  u8a = u8b + u8c + u8d; /* Same essential type */
  u8a = ( uint8_t ) s8a; /* Cast gives same essential type */
  u32a = u16a; /* Assignment to a wider essential type */
  u32a = 2U + 125U; /* Assignment to a wider essential type */
  //use_uint16 ( u8a ); /* Assignment to a wider essential type */
  //use_uint16 ( u8a + u16b ); /* Assignment to same essential type */
  float _Complex cf32a;
  float f32a;
  
  cf32a = f32a; /* By exception 4 */
  //cf64a = f64a; /* By exception 4 */
  
  uint8_t u8f = f32a; // UndCC_Violation
  bool bla = s8a; // UndCC_Violation
  char cha1 = s8a; // UndCC_Violation
  u8a = cha; // UndCC_Violation
  u8b = u8a - s8a; // UndCC_Violation
  u8c += cha; // UndCC_Violation
  //use_uint32 ( s32a ); /* signed and unsigned */
  //f32a = cf32a; /* real floating and complex floating */
}

void booleanKeywords()
{
  bool b = false;               // UndCC_Valid
  int i = true;                 // UndCC_Violation - Boolean assigned to a signed object
  b = true;                     // UndCC_Valid
  (void)b; (void)i;
}

unsigned char namedCasts(int v, const unsigned char *p)
{
  unsigned char u;
  int s;
  u = static_cast<unsigned char>(v);          // UndCC_Valid - the cast gives the essential type
  u = static_cast<unsigned char>(v) + 1U;     // UndCC_Valid
  s = static_cast<int>(u);                    // UndCC_Valid
  s = static_cast<unsigned int>(v);           // UndCC_Violation - unsigned assigned to signed
  u = *const_cast<unsigned char *>(p);        // UndCC_Valid
  (void)s;
  return static_cast<unsigned char>(v);       // UndCC_Valid
}

template <typename T> struct Limits { static T max(); };
template <typename T> int widthOf(T v);
template <typename T> T convert(int v);

void templateArguments(int v)
{
  unsigned char u = 0U;
  int s;
  u = Limits<unsigned char>::max();           // UndCC_Valid - the template arguments are types, not comparisons
  s = widthOf<unsigned char>(u);              // UndCC_Valid - returns int
  u = widthOf<unsigned char>(u);              // UndCC_Violation - int assigned to unsigned char
  u = convert<unsigned char>(v);              // UndCC_Valid
  s = v < 2 > 0;                              // UndCC_Violation - relational operators, not a template
  (void)s;
}
