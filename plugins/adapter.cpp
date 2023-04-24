#include "adapter.h"
#include "../widget.h"
#include "../button.h"
#include "../error.h"


int ButtonAdapterController (Button* button, WidgetManager* widget)
{
    START_;

    ButtonAdapter* button_adapter = (ButtonAdapter*) button;
    Point click = widget->get_main_widget_()->get_click_coordinate();
    button_adapter->ibutton_->on_mouse_press({(int)click.x, (int)click.y});

    Tool* tool = button->get_tool();
    if (tool == NULL)
    {
        PRINT_("button's tool null ptr\n");
        END_(-1);
    }
    if (tool->set_active_tool_in_manager())
    {
        PRINT_("button's tool cant become active\n");
        END_(-1);
    }
    else
    {
        widget->set_active_tool_manager((ToolManager*)tool->get_tool_manager());

        widget->repaint_all_with_state(CurrentWork::ChangeActiveTool);

        END_(0);
    }
    END_(0);
}

int ButtonAdapterPaint(Button* button)
{
    START_;

    ButtonAdapter* button_adapter = (ButtonAdapter*) button;

    Point start {button->get_start_point()};
    Point end {button->get_end_point()};

    int w = end.x - start.x;
    int h = end.y - start.y;

    button_adapter->ibutton_->draw((unsigned char*)button_adapter->get_bit_array(), w, h);

    button_adapter->get_layer()->paint_from_bits(button_adapter);

    END_(0);
}

int WidgetAdapterPaint(WidgetManager* widget)
{
    START_;

    WidgetAdapter* widget_adapter = (WidgetAdapter*) widget;
    int size_x = widget_adapter->iwidget_->get_size().x;
    int size_y = widget_adapter->iwidget_->get_size().y;

    int x0 = widget_adapter->iwidget_->get_pos().x;
    int y0 = widget_adapter->iwidget_->get_pos().y;

    
/*
    QRect target(x0, y0, size_x, size_y);
    //QRect target(0, 0, 1920, 1080);
    uchar* collor_buffer = widget_adapter->src_.bits();

    widget_adapter->iwidget_->draw((unsigned char*)collor_buffer, size_x,
                                                                 size_y);

    QPixmap pmap(size_x, size_y);
    pmap.convertFromImage(widget_adapter->src_);

    painter->drawPixmap(target, pmap, target);
*/
    END_(0);
}

int controller_adapter_widget (Button* button, WidgetManager* widget)
{
    START_;
    if (button)
    {
        button->response_(button, widget);
    }
    if (widget)
    {
        //Point click = widget->get_main_widget_()->get_click_coordinate();

        (widget->get_main_widget_())->repaint_widget();
    }
    END_(0);
}

int ToolActivityAdapter(Tool* tool, LayerObject* painter, Point p)
{
    START_;

    ToolAdapter* tool_adapter = (ToolAdapter*) tool;

    Point end = tool_adapter->paint_widget_->get_end_point();
    Point start = tool_adapter->paint_widget_->get_start_point();

    int size_x = end.x - start.x;//tool_adapter->iwidget_->get_size().x;
    int size_y = end.y - start.y;//tool_adapter->iwidget_->get_size().y;

    int x0 = start.x;
    int y0 = start.y;

    WidgetManager* main_widget = tool_adapter->paint_widget_->get_main_widget_();

    ColorPl<double> color = {tool_adapter->get_color().r, tool_adapter->get_color().g, tool_adapter->get_color().b};

    tool_adapter->itool_->set_color(color);
    tool_adapter->itool_->set_thickness(tool_adapter->get_thickness());

    tool_adapter->itool_->apply((unsigned char*)tool_adapter->paint_widget_->get_bit_array(), size_x, size_y,
                                {(int)p.x - x0, (int)p.y - y0});

    tool_adapter->paint_widget_->get_layer()->paint_from_bits(tool_adapter->paint_widget_);

    END_(0);
}
