#include "tool.h"
#include "error.h"
#include "widget.h"

int paint_dot (Tool* tool, LayerObject* object, Point click)
{
    START_;

    object->get_layer()->paint_dot(object, click, tool->get_thickness() ,tool->get_color());

    tool->set_last_click(click);

    END_(0);
}

#include <math.h>

int paint_line (Tool* tool, LayerObject* object, Point click)
{
    START_;
    tool->add_cur_num_click();

    if (tool->get_cur_num_click() == 2)
    {

        object->get_layer()->paint_line(object, tool->get_last_click(), click, tool->get_thickness(), tool->get_color());

        tool->set_cur_num_click(0);

    }

    tool->set_last_click(click);

    END_(0);
}

int clear_dot (Tool* tool, LayerObject*, Point click)
{
    START_;
    /*Color color = {1, 1, 1};
    QPen paintpen(QColor(color.r * 255, color.g * 255, color.b * 255));
    paintpen.setWidth(tool->get_thickness());
    p->setPen(paintpen);
    p->drawPoint(click.x, click.y);*/

    tool->set_last_click(click);

    END_(0);
}

int Tool::set_active_tool_in_manager()
{
    START_;
    if (my_tool_manager_)
    {
        ToolManager* tools = (ToolManager*) my_tool_manager_;
        tools->set_active_tool(this);

        END_(0);
    }

    END_(-1);
}
