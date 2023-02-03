#ifndef VECTORS_H
#define VECTORS_H
#include "math.h"

#define square(num) (num * num)

struct Color
{
    double r = 0;
    double g = 0;
    double b = 0;
};

//static const double EPS = 10e-8;

struct Point
{
    double x = 0;
    double y = 0;
    double z = 0;
};

#endif // VECTORS_H
