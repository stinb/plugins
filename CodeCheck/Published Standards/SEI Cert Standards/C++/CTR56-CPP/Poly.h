#ifndef POLY_H
#define POLY_H

int globI;
double globD;

struct S
{
    int i;

    S() : i(globI++) {}
};

struct T : S
{
    double d;

    T() : S(), d(globD++) {}
};

#endif POLY_H