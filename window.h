#ifndef WINDOW_H
#define WINDOW_H

#include <SFML/Graphics.hpp>
#include "math.h"
#include "layer.h"


class CoordinateSystem
{
private:
    Point start_point_ = {};
    Point end_point_ = {};
    Point origin_point_ = {};

public:
    CoordinateSystem(Point start, Point end, Point origin):
        start_point_(start),
        end_point_(end),
        origin_point_(origin)
    {
    }
    CoordinateSystem(Point start, Point end):
        start_point_(start),
        end_point_(end)
    {
    }
    CoordinateSystem(){}
    //~CoordinateSystem(){}

    int heigh() const
    {
        return end_point_.y - start_point_.y;
    }

    int width() const
    {
        return end_point_.x - start_point_.x;
    }

    Point get_origin_point() const
    {
        return origin_point_;
    }
    Point get_start_point()const
    {
        return start_point_;
    }
    Point get_end_point()const
    {
        return end_point_;
    }
    int is_my_area(Point click) const;
    int paintCoordinateSystem(WidgetManager*, bool = false, Color = {}, Color = {});
};

class LayerObject : public CoordinateSystem
{
private:
    Layer* layer_ = NULL;
    char* bit_array_ = NULL;
public:
    LayerObject(Point a, Point b, Layer* layer):
        CoordinateSystem(a, b)
    {
        bit_array_ = (char*) calloc ((b.y - a.y) * (b.x - a.x) * 3, 0);
        set_layer(layer);
    }
    char* get_bit_array(){return bit_array_;}
    
    int set_layer(Layer* layer){layer_ = layer; return layer->add_object(this);}
    Layer* get_layer(void){return layer_;}
};

/*
class Window_Clock : public Window
{
private:
    double radius_ = 0;
protected:
    void paintEvent(QPaintEvent *) override;
public:
    Window_Clock(Point start, Point end, Point origin):
        Window(start, end, origin){};

    void set_radius(double radius)
    {
        radius_ = radius;
    }
    double get_radius()
    {
        return radius_;
    }
};

class Window_Click : public Window
{
private:
     Point mouse_click_ = {};
protected:
    void paintEvent(QPaintEvent *) override;
    void mousePressEvent(QMouseEvent *) override;
public:
    Window_Click(Point start, Point end, Point origin):
        Window(start, end, origin){};
};*/


#endif // WINDOW_H
