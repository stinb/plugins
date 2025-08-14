float f1, f2;
int _Atomic volatile i1, i2;
char c1, c2;
void *p1, *p2;
_Complex float cf1, cf2;
unsigned x;

void fn1(void)
{
    f2 = sqrt(f1); /* UndCC_Valid - essentially floating real type */
    i2 = sqrt(i1); /* UndCC_Valid - essentially integer real type */
    c2 = sqrt(c1); /* UndCC_Violation - essentially character real type */
    /* p2 = sqrt(p1); Non-compliant, strict parser error - undefined behaviour */
}

void fn2(void)
{
    f2 = sqrt(f1);   /* UndCC_Valid - real argument */
    cf2 = sqrt(cf1); /* UndCC_Valid - sqrt has a complex equivalent */
    f2 = ceil(f1);   /* UndCC_Valid - real argument */
    cf2 = ceil(cf1); /* UndCC_Violation - undefined behaviour */
}
