#include "window.h"
#include "math.h"

int CoordinateSystem::paintCoordinateSystem (QPainter* painter, bool with_area, Color color_line, Color color_area)
{
    if (painter == NULL)
    {
        return -1;
    }
    int x0 = start_point_.x;
    int y0 = start_point_.y;

    int height = heigh();
    int widtht = width();

    QBrush brush_save  = painter->brush();

    if (with_area == true)
    {
        QBrush brush(QColor(color_area.r * 255, color_area.g * 255, color_area.b * 255));
        painter->setBrush(brush);
    }

    QPen paintpen(QColor(color_line.r * 255, color_line.g * 255, color_line.b * 255));
    painter->setPen(paintpen);

    painter->drawRect(x0, y0, widtht, height);

    painter->setBrush(brush_save);
    return 0;
}

/*
void Window_Clock::paintEvent(QPaintEvent *)
{
    static double alpha = 0;

    QPainter painter(this);

    Point origin_point = get_origin_point();
    paintCoordinateSystem(&painter);

    Vector vec = {{0, 0}, {get_radius() * sin(alpha), get_radius() * cos(alpha)}};
    Vector vec2 = vec - vec * 2;
    vec.paintVector(&painter, origin_point);
    vec2.paintVector(&painter, origin_point);

    alpha += 3.14/10000;
    update();
}


void Window_Click::mousePressEvent(QMouseEvent *event)
{
    Point origin_point = get_origin_point();
    mouse_click_.x = event->x() - origin_point.x;
    mouse_click_.y = origin_point.y - event->y();
    repaint();
}


void Window_Click::paintEvent(QPaintEvent *)
{
    QPainter painter(this);

    Point origin_point = get_origin_point();
    paintCoordinateSystem(&painter);

    Vector vec = {{0, 0}, {mouse_click_.x, mouse_click_.y}};

    vec.paintVector(&painter, origin_point);
}*/

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
