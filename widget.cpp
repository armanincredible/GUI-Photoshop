#include "widget.h"
#include "button.h"
#include "tool.h"
#include "error.h"
//#include "texteditor.h"

//WidgetManager* WidgetManager::cur_widget_painting_ = NULL;


int StandardWidgetPaint(WidgetManager* widget)
{
    START_;
    /*
     * widget which we paint can be only active, non active and prev active
     * if its prev active we repaint his lines in typical color
     */
    /*if (widget == widget->get_active_widget())
    {
        if (widget->get_prev_active_widget())
        {
            widget->get_prev_active_widget()->paintCoordinateSystem(painter);
        }
        widget->paintCoordinateSystem(painter, false, {0.2, 0.6, 0.2});
    }
    else
    {
        widget->paintCoordinateSystem(painter);
    }*/

    widget->get_layer()->paint_rectangle(widget);

    for (int i = 0; i < widget->get_widgets_num(); i++)
    {
        WidgetManager* cur_widget = (widget->get_widgets())[i];
        cur_widget->paint_function_(cur_widget);
        //widgets_[i]->paint(painter);
    }

    for (int i = 0; i < widget->get_buttons_num(); i++)
    {
        Button* cur_button = (widget->get_buttons())[i];
        cur_button->paint_function_(cur_button);
        //cur_button->paintCoordinateSystem(painter);
    }
    END_(0);
}

/*
 * Function find object, which area was clicked and call controller
 * If controller was called function return 0, another way -1
 */
int WidgetManager::click_handler(Point click)
{
    START_;
    WidgetManager* widget = NULL;
    for (int i = 0; i < widgets_num_; i++)
    {
        widget = widgets_[i];
        Button* button = NULL;
        Button** buttons = widget->get_buttons();
        for (int i = 0; i < widget->get_buttons_num(); i++)
        {
            button = buttons[i];
            if (button->is_my_area(click))
            {
                PRINT_("found me button in widget %p\n", widget);
                if (widget->controller_)
                {
                    if (widget->last_activity_)
                    {
                        widget->last_activity_(widget);
                    }
                    set_active_widget(widget);
                    widget->controller_(button, widget);
                }
                else
                {
                    PRINT_("contoller function null ptr\n");
                }
                END_(0);
            }
        }

        if (widget->is_my_area(click))
        {
            PRINT_("found me widget %p\n", widget);
            if (!widget->click_handler(click))
            {
                END_(0);
            }

            widget->set_click_coordinate(click);
            if (widget->controller_)
            {
                if (widget->last_activity_)
                {
                    widget->last_activity_(widget);
                }
                set_active_widget(widget);
                widget->controller_(NULL, widget);
            }
            else
            {
                PRINT_("contoller function null ptr\n");
            }
            END_(0);
        }
        //widget->click_handler(click);
    }
    PRINT_("didnt find anything\n");
    //paint_function_(this);
    END_(-1);
}

void WidgetManager::keyPressEvent(sf::Event *event)
{
    START_;
    //PRINT_("%p is this widget", this);
    WidgetManager* active_widget = get_active_widget();
    if (active_widget && active_widget->is_need_in_key_events())
    {
            active_widget->set_key_event(&event->key);
            active_widget->controller_(NULL, active_widget);
    }
    END_();
}

void WidgetManager::paintEvent()
{
    START_;
    WidgetManager* widget = NULL;

    static bool have_background = false;
    if (!have_background)
    {
        PRINT_("i'm main widget and paint white backside\n");
        get_layer()->paint_rectangle_with_area(this, {1, 1, 1});
        have_background = true;
    }

    if (active_widget_)
    {
        PRINT_("Have active widget\n");
        widget = active_widget_;
    }
    else
    {
        PRINT_("Haven't active widget\n");
        widget = this;
    }

    /*
     * for this stage this method know only about active widget
     * if it wants to paint it needs to have tool manager
     */
    if (widget->get_tool_manager())
    {
        PRINT_("Widget has tool manager\n");
        Tool* tool = get_active_tool_from_tool_manager();
        if (tool)
        {
            tool->activity_(tool, widget, get_click_coordinate());
        }
    }
    else
    {
        /*
         * paint only active widget or all widgets if active is null
         */
        PRINT_("Widget hasn't tool manager\n");
        widget->paint_function_(widget);
    }
    END_();
}

int WidgetManager::repaint_all_with_state(CurrentWork state)
{
    START_;

    WidgetManager* saved_widget = get_active_widget();
    set_active_widget(NULL);

    repaint_with_state(state);

    set_active_widget(saved_widget);

    END_(0);
}

int WidgetManager::repaint_with_state(CurrentWork state)
{
    START_;

    CurrentWork state_saved = get_work_state();
    set_work_state(state);

    (get_main_widget_())->repaint_widget();

    set_work_state(state_saved);

    END_(0);
}

void WidgetManager::mouseReleaseEvent(sf::Event *)
{
    START_;
    is_mouse_pressed_ = false;
    END_();
}

void WidgetManager::mouseMoveEvent(sf::Event *event)
{
    START_;
    Point click = Point{double(event->mouseMove.x), double(event->mouseMove.y)};
    set_click_coordinate(click);
    if (is_mouse_pressed_)
    {
        click_handler(click);
    }
    END_();
}

void WidgetManager::timerEvent(sf::Time time)
{
    START_;

    CurrentWork state_saved = get_work_state();
    set_work_state(CurrentWork::TimerReaction);

    WidgetManager** widgets = get_widgets_with_timer();
    int num = get_widgets_with_timer_num();
    for (int i = 0; i < num; i++)
    {
        WidgetManager* widget = widgets[i];
        //widgets[i]->set_last_time(time.asMilliseconds());
        if (widget->get_timer() <= time.asMilliseconds() + widget->get_passed_time())
        {
            widget->set_passed_time(0);

            if (widget->timer_controller_)
            {
                widget->timer_controller_(widget);
            }
        }
        else
        {
            widget->add_passed_time(time.asMilliseconds());
        }
    }

    set_work_state(state_saved);
    END_();
}

void WidgetManager::mousePressEvent(sf::Event *event)
{
    START_;
    is_mouse_pressed_ = true;
    Point click = Point{double(event->mouseButton.x), double(event->mouseButton.y)};
    set_click_coordinate(click);
    click_handler(click);
    END_();
}

WidgetManager *WidgetManager::get_main_widget_()
{
    START_;
    static WidgetManager* widget;
    if (!widget)
    {
        widget = this;
        while(widget->get_parent_widget())
        {
            widget  = widget->get_parent_widget();
        }
    }
    END_(widget);
}


int controller_paint (Button* button, WidgetManager* widget)
{
    START_;
    if (button)
    {
        button->response_(button, widget);
        END_(0);
    }
    if (widget)
    {
        PRINT_("going to repaint, %p\n", widget);

        (widget->get_main_widget_())->repaint_widget();
    }

    END_(0);
}

int controller_only_buttons (Button* button, WidgetManager* widget)
{
    START_;
    if (button)
    {
        button->response_(button, widget);
        END_(0);
    }
    if (widget)
    {
        PRINT_("going to repaint, %p\n", widget);

        (widget->get_main_widget_())->repaint_widget();
    }
    END_(0);
}

int timer_controller_main_widget(WidgetManager* widget)
{
    START_;
    static bool paint_line = true;
    if (widget == widget->get_active_widget())
    {
        (widget->get_main_widget_())->repaint_widget();
    }
    END_(0);
}
