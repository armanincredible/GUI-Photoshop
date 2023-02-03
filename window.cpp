#include "window.h"
#include "math.h"
#include "widget.h"

int CoordinateSystem::paintCoordinateSystem (WidgetManager* widget, bool with_area, Color color_line, Color color_area)
{
    if (widget == NULL)
    {
        return -1;
    }
    int x0 = start_point_.x;
    int y0 = start_point_.y;

    int height = heigh();
    int widtht = width();

    sf::RectangleShape rectangle(sf::Vector2f(widtht, height));
    rectangle.setPosition(sf::Vector2f(x0, y0));

    //rectangle.setFillColor(sf::Color(100, 250, 50));
    rectangle.setOutlineColor(sf::Color(0, 0, 0));
    rectangle.setOutlineThickness(10);

    /*QBrush brush_save  = painter->brush();

    if (with_area == true)
    {
        QBrush brush(QColor(color_area.r * 255, color_area.g * 255, color_area.b * 255));
        painter->setBrush(brush);
    }

    QPen paintpen(QColor(color_line.r * 255, color_line.g * 255, color_line.b * 255));
    painter->setPen(paintpen);

    painter->drawRect(x0, y0, widtht, height);

    painter->setBrush(brush_save);*/
    widget->get_main_widget_()->draw(rectangle);
    return 0;
}


int CoordinateSystem::is_my_area(Point click) const
{
    Point start_point = get_start_point();
    Point end_point = get_end_point();
    //fprintf (stderr, "start x and y %lg %lg\n", start_point.x, start_point.y);
    //fprintf (stderr, "end x and y %lg %lg\n", end_point.x, end_point.y);
    if (click.x > start_point.x && click.x < end_point.x &&
        click.y > start_point.y && click.y < end_point.y)
    {
        return true;
    }
    else
    {
        return false;
    }
}
