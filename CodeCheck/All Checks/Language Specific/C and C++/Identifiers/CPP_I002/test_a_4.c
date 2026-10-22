// Custom

// Struct tags and typedef names have no linkage
typedef struct a_tag1_t a_tag1_t; // UndCC_Valid

struct a_tag1_t
{
 int a_m1;
};

typedef struct a_tag2_t // UndCC_Valid
{
 int a_m2;
} a_tag2_t;

void a_tags ( void )
{
 typedef struct a_tag3_t a_tag3_t; // UndCC_Valid

 struct a_tag3_t
 {
  int a_m3;
 };

 typedef struct a_tag4_t // UndCC_Valid
 {
  int a_m4;
 } a_tag4_t;
}

//               1234567890123456789012345678901*********     Characters
typedef int      a_gggggggggggggggggggggggggggg0_t; // UndCC_Valid - a type, not an object
int              a_gggggggggggggggggggggggggggg1_t;
