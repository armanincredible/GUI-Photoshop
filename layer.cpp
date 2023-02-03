#include "layer.h"
#include "window.h"
#include "widget.h"
#include "error.h"


int Layer::paint_rectangle(LayerObject* widget, WidgetManager* painter)
{
    START_;
    widget->paintCoordinateSystem(painter);
    END_(0);
}

int Layer::paint_rectangle_with_area(LayerObject* widget, WidgetManager* painter, Color color)
{
    START_;
    widget->paintCoordinateSystem(painter, true, {0, 0, 0}, {color});
    END_(0);
}
