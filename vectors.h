#ifndef VECTORS_H
#define VECTORS_H
#include "QPainter"
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

class Vector
{
private:
    Point out_point_ = {};
    Point in_point_ = {};

    Point first_point_arrow = {};
    Point second_point_arrow = {};

    Point real_out_point_ = {};
    Point real_in_point_  = {};
public:
    Vector(Point out, Point in):
        out_point_(out),
        in_point_(in)
    {}
    Vector(){}
    Point get_out_point() const
    {
        return out_point_;
    }
    Point get_in_point() const
    {
        return in_point_;
    }
    Point get_real_in_point() const
    {
        return real_in_point_;
    }
    Point get_real_out_point() const
    {
        return real_out_point_;
    }
    void change_points(Point out, Point in)
    {
        out_point_ = out;
        in_point_  = in;
    }
    double get_length_x() const
    {
        return in_point_.x - out_point_.x;
    }
    double get_length_z() const
    {
        return in_point_.z - out_point_.z;
    }
    double get_length_y() const
    {
        return in_point_.y - out_point_.y;
    }
    double get_sqaure_length() const
    {
        return square(get_length_x()) + square(get_length_y()) + square(get_length_z());
    }

    void CalculateRealPoints(Point);
    void CalculateArrow(Point);
    int paintVector(QPainter* painter, Point);
    void NormalizeVector();
    void Rotate(double, double);
    void RotateOnRandomCorner();

    Vector operator + (const Vector &);
    Vector operator - (const Vector &);
    Vector operator ^ (const Vector &);
    Vector operator | (const Vector &);
    Vector& operator += (const Vector &);
    Vector& operator -= (const Vector &);
    double operator * (const Vector &);
    Vector operator * (const double);
    Vector operator / (const double);
    Vector& operator *= (const double);
};

double get_cos (Vector&, Vector&);

#endif // VECTORS_H
