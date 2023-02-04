#include "window.h"
#include "math.h"
#include "widget.h"


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
