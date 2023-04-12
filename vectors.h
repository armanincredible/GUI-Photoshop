#ifndef VECTORS_H
#define VECTORS_H
#include "math.h"

#define square(num) (num * num)

struct Color
{
    double r = 0;
    double g = 0;
    double b = 0;

    bool operator == (Color& color)
    {
        return (this->r == color.r) && (this->g == color.g) && (this->b == color.b);
    }
    bool operator != (Color& color)
    {
        return (!(*this == color));
    }
};

template <typename T>
struct IColor
{
    T r = 0;
    T g = 0;
    T b = 0;

    bool operator == (IColor& color)
    {
        return (this->r == color.r) && (this->g == color.g) && (this->b == color.b);
    }
    bool operator != (IColor& color)
    {
        return (!(*this == color));
    }
};

//static const double EPS = 10e-8;

struct Point
{
    double x = 0;
    double y = 0;
    double z = 0;
};

#endif // VECTORS_H
