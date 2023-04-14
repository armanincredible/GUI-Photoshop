#ifndef WIDGET_H
#define WIDGET_H
#include "coordinate_system.h"
#include "layer_object.h"
#include "button.h"
#include "tool.h"
#include <SFML/Graphics.hpp>

enum TypeWidget
{
    RedColorProperty,
    GreenColorProperty,
    BlueColorProperty,
};

enum CurrentWork
{
    ChangeActiveTool,
    TimerReaction,
    Nothing
};

class AbstrWidget : public LayerObject, public sf::RenderWindow
{
    virtual void paintEvent(){};
    virtual void mousePressEvent(sf::Event *){};
    virtual void mouseReleaseEvent(sf::Event *){};
    virtual void mouseMoveEvent(sf::Event *){};
    virtual void keyPressEvent(sf::Event *){};
    virtual void timerEvent(sf::Time){};
public:
    void resize_widget(int a, int b)
    {
        sf::Vector2u vec(a, b); 
        setSize(vec);
    }
    void show_widget(){display();}

    AbstrWidget(Point a, Point b, Layer* layer):
        LayerObject(a, b, layer),
        sf::RenderWindow()
    {}
};

class WidgetManager : public AbstrWidget
{
private:
    CurrentWork cur_work_state_ = CurrentWork::Nothing;

    sf::Event::KeyEvent* key_event_ = NULL;
    Point mouse_click_coordinate_ = {};
    bool is_mouse_pressed_ = 0;

    bool need_in_key_events_ = false;

    WidgetManager** widgets_ = NULL;
    size_t widgets_num_ = 0;

    WidgetManager** widgets_with_timer_ = NULL;
    size_t widgets_with_timer_num_ = 0;

    ToolManager* tool_manager_ = NULL;
    size_t tools_num_ = 0;

    Button** buttons_ = NULL;
    size_t buttons_num_ = 0;

    WidgetManager* parent_widget_ = NULL;
    char* widget_name = NULL;

    WidgetManager* prev_active_widget_ = NULL;
    WidgetManager* active_widget_ = NULL;
    ToolManager* active_tool_manager_ = NULL;

    int timer_ = 0;
    int passed_time_ = 0;
    bool is_has_timer_ = false;

public:
    void keyPressEvent(sf::Event *) override;
    void paintEvent() override;
    void mousePressEvent(sf::Event *) override;
    void mouseReleaseEvent(sf::Event *) override;
    void mouseMoveEvent(sf::Event *) override;
    void timerEvent(sf::Time) override;

    int repaint_widget(){
        get_main_widget_()->paintEvent();
        return 0;
    }

    int (*controller_) (Button*, WidgetManager*) = NULL;
    int (*paint_function_)(WidgetManager*) = NULL;
    int (*timer_controller_) (WidgetManager*) = NULL;
    int (*last_activity_) (WidgetManager*) = NULL;

    void set_timer_controller(int (*timer_controller) (WidgetManager*))
    {
        timer_controller_ = timer_controller;
    }

    void set_timer(int millisec){timer_ = millisec;}
    int get_timer(){return timer_;}
    void set_passed_time(int passed_time){passed_time_ = passed_time;}
    void add_passed_time(int passed_time){passed_time_ += passed_time;}
    int get_passed_time(){return passed_time_;}

    CurrentWork get_work_state(void){return get_main_widget_()->cur_work_state_;}
    void set_work_state(CurrentWork state){get_main_widget_()->cur_work_state_ = state;}

    bool is_need_in_key_events(){return need_in_key_events_;}
    void set_need_in_key_events(bool val){need_in_key_events_ = val;}

    WidgetManager(Point start_point, Point end_point,
                  WidgetManager* parent_widget,
                  int (*controller) (Button*, WidgetManager*),
                  int (*paint_func) (WidgetManager*),
                  Layer* layer):
        parent_widget_(parent_widget),
        controller_(controller),
        AbstrWidget(start_point, end_point, layer),
        paint_function_(paint_func)
    {
        if (!parent_widget)
        {
            create(sf::VideoMode(end_point.x - start_point.x, end_point.y - start_point.y), "MainWindow");
        }
    }

    WidgetManager(Point start_point, Point end_point,
                  WidgetManager* parent_widget,
                  int (*paint_func) (WidgetManager*),
                  Layer* layer):
        WidgetManager(start_point, end_point, parent_widget, NULL, paint_func, layer)
    {}

    ~WidgetManager()
    {
        if (widgets_)
        {
            free(widgets_);
        }
        if (buttons_)
        {
            free(buttons_);
        }
        if (is_has_timer_)
        {
            //killTimer(timerId_);
        }
    }

    int add_widget_on_timer(WidgetManager* widget)
    {
        if (!widgets_with_timer_num_ && widgets_with_timer_)
        {
            printf ("ERROR %d\n", __LINE__);
        }
        widgets_with_timer_num_++;
        widgets_with_timer_ = (WidgetManager**) realloc (widgets_with_timer_, widgets_with_timer_num_ * sizeof(WidgetManager*));
        if (widgets_with_timer_ == NULL)
        {
            return -1;
        }
        widgets_with_timer_[widgets_with_timer_num_ - 1] = widget;
        return 0;
    }
    size_t get_widgets_with_timer_num (){return widgets_with_timer_num_;}
    WidgetManager** get_widgets_with_timer (){return widgets_with_timer_;}

    int add_widget (WidgetManager* widget)
    {
        if (!widgets_num_ && widgets_)
        {
            printf ("ERROR %d\n", __LINE__);
        }
        widgets_num_++;
        widgets_ = (WidgetManager**) realloc (widgets_, widgets_num_ * sizeof(WidgetManager*));
        if (widgets_ == NULL)
        {
            return -1;
        }
        widgets_[widgets_num_ - 1] = widget;
        widget->set_parent_widget(this);
        return 0;
    }
    WidgetManager** get_widgets (){return widgets_;}
    void set_widgets (WidgetManager** widgets){widgets_ = widgets;}
    void set_widgets_num (size_t num){widgets_num_ = num;}
    size_t get_widgets_num (){return widgets_num_;}

    int add_button (Button* button)
    {
        if (!buttons_num_ && buttons_)
        {
            printf ("ERROR %d\n", __LINE__);
        }
        if (!button)
        {
            return -1;
        }
        buttons_num_++;
        buttons_ = (Button**) realloc (buttons_, buttons_num_ * sizeof(Button*));
        if (buttons_ == NULL)
        {
            return -1;
        }
        buttons_[buttons_num_ - 1] = button;
        button->set_widget(this);
        return 0;
    }
    Button** get_buttons (){return buttons_;}
    void set_buttons (Button** buttons){buttons_ = buttons;}
    void set_buttons_num (size_t num){buttons_num_ = num;}
    size_t get_buttons_num (){return buttons_num_;}

    WidgetManager* get_parent_widget (){return parent_widget_;}
    void set_parent_widget (WidgetManager* widget){parent_widget_ = widget;}

    ToolManager* get_tool_manager (){return tool_manager_;}
    void set_tool_manager (ToolManager* tool_manager){tool_manager_ = tool_manager;}

    WidgetManager* get_main_widget_ ();

    WidgetManager* get_active_widget (){return get_main_widget_()->active_widget_;}
    void set_active_widget (WidgetManager* tool_manager)
    {
        get_main_widget_()->prev_active_widget_ = get_main_widget_()->active_widget_;
        get_main_widget_()->active_widget_ = tool_manager;
    }

    WidgetManager* get_prev_active_widget (){return get_main_widget_()->prev_active_widget_;}
    void set_prev_active_widget (WidgetManager* tool_manager){get_main_widget_()->prev_active_widget_ = tool_manager;}

    ToolManager* get_active_tool_manager (){return get_main_widget_()->active_tool_manager_;}
    void set_active_tool_manager (ToolManager* tool_manager){get_main_widget_()->active_tool_manager_ = tool_manager;}

    Tool* get_active_tool_from_tool_manager()
    {
        if (get_active_tool_manager())
        {
            return get_active_tool_manager()->get_active_tool();
        }
        else
        {
            return NULL;
        }
    }
    int set_active_tool_from_tool_manager(Tool* tool)
    {
        if (get_active_tool_manager())
        {
            get_active_tool_manager()->set_active_tool(tool);
            return 0;
        }
        else
        {
            return -1;
        }
    }

    bool is_mouse_pressed (){return is_mouse_pressed_;}

    Point get_click_coordinate (){return mouse_click_coordinate_;}
    void set_click_coordinate (Point click){mouse_click_coordinate_ = click;}

    void set_key_event(sf::Event::KeyEvent* event){key_event_ = event;}
    sf::Event::KeyEvent* get_key_event(void){return key_event_;}

    int click_handler(Point);

    int repaint_all_with_state(CurrentWork);
    int repaint_with_state(CurrentWork);
};

int controller_paint (Button*, WidgetManager*);
int StandardWidgetPaint(WidgetManager*);

#endif // WIDGET_H
