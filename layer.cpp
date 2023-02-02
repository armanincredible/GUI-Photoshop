#include "layer.h"
#include "window.h"
//#include "widget.h"


int Layer::paint_rectangle(LayerObject* widget, QPainter* painter)
{
    widget->paintCoordinateSystem(painter);
    return 0;
}

int Layer::paint_rectangle_with_area(LayerObject* widget, QPainter* painter, Color color)
{
    widget->paintCoordinateSystem(painter, true, {0, 0, 0}, {color});
    return 0;
}
