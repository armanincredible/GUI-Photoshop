#include "tool.h"
#include "error.h"

int paint_dot (Tool* tool, QPainter* p, Point click)
{
    START_;
    Color color = tool->get_color();
    QPen paintpen(QColor(color.r * 255, color.g * 255, color.b * 255));
    paintpen.setWidth(tool->get_thickness());
    p->setPen(paintpen);
    p->drawPoint(click.x, click.y);
    //p->pen();
    //p->drawLine(10, 20, 40, 50);

    tool->set_last_click(click);

    END_(0);
}

int paint_line (Tool* tool, QPainter* p, Point click)
{
    START_;
    if (tool->get_cur_num_click())
    {
        Color color = tool->get_color();
        QPen paintpen(QColor(color.r * 255, color.g * 255, color.b * 255));
        paintpen.setWidth(tool->get_thickness());
        p->setPen(paintpen);
        p->drawLine(tool->get_last_click().x, tool->get_last_click().y,
                    click.x, click.y);
        tool->set_cur_num_click(0);

        END_(0);
    }

    tool->add_cur_num_click();
    tool->set_last_click(click);

    END_(0);
}

int clear_dot (Tool* tool, QPainter* p, Point click)
{
    START_;
    Color color = {1, 1, 1};
    QPen paintpen(QColor(color.r * 255, color.g * 255, color.b * 255));
    paintpen.setWidth(tool->get_thickness());
    p->setPen(paintpen);
    p->drawPoint(click.x, click.y);

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
