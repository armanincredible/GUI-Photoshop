#ifndef WIDGET_H
#define WIDGET_H
#include "coordinate_system.h"
#include "layer_object.h"
#include "button.h"
#include "tool.h"
#include <SFML/Graphics.hpp>
#include <stdlib.h>
#include <SFML/Audio.hpp>
#include <iostream>
#include "error.h"

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

    std::vector <WidgetManager*> widgets_;
    std::vector <WidgetManager*> widgets_with_timer_;

    ToolManager* tool_manager_ = NULL;
    size_t tools_num_ = 0;

    std::vector <Button*> buttons_;

    WidgetManager* parent_widget_ = NULL;
    char* widget_name = NULL;

    WidgetManager* prev_active_widget_ = NULL;
    WidgetManager* active_widget_ = NULL;
    ToolManager* active_tool_manager_ = NULL;

    int timer_ = 0;
    int passed_time_ = 0;
    bool is_has_timer_ = false;

    sf::Music music_;

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

    int play_music()
    {
        START_;
        if (!music_.openFromFile("stream/da.ogg"))
        {
            PRINT_("Failed to load music: ");
            END_(-1);
        }

        music_.play();
        music_.setVolume(50);
        END_(0);
    }

    int change_music_volume(float val)
    {
        START_;
        music_.setVolume(val);
        END_(0);
    }

    float get_music_volume()
    {
        START_;
        END_(music_.getVolume());
    }

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
        if (is_has_timer_)
        {
            //killTimer(timerId_);
        }
    }

    int add_widget_on_timer(WidgetManager* widget)
    {
        if (!widget)
        {
            printf ("ERROR %d\n", __LINE__);
            return -1;
        }
        widgets_with_timer_.push_back(widget);
        widget->set_parent_widget(this);
        return 0;
    }
    size_t get_widgets_with_timer_num (){return widgets_with_timer_.size();}
    WidgetManager** get_widgets_with_timer (){return widgets_with_timer_.data();}

    int add_widget (WidgetManager* widget)
    {
        if (!widget)
        {
            printf ("ERROR %d\n", __LINE__);
            return -1;
        }
        widgets_.push_back(widget);
        widget->set_parent_widget(this);
        return 0;
    }

    WidgetManager** get_widgets (){return widgets_.data();}
    //void set_widgets (WidgetManager** widgets){widgets_ = widgets;}
    //void set_widgets_num (size_t num){widgets_num_ = num;}
    size_t get_widgets_num (){return widgets_.size();}

    int add_button (Button* button)
    {
        if (!button)
        {
            printf ("ERROR %d\n", __LINE__);
            return -1;
        }
        buttons_.push_back(button);
        button->set_widget(this);
        return 0;
    }
    Button** get_buttons (){return buttons_.data();}
    //void set_buttons (Button** buttons){buttons_ = buttons;}
    //void set_buttons_num (size_t num){buttons_num_ = num;}
    size_t get_buttons_num (){return buttons_.size();}

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
int controller_with_volume_button (Button*, WidgetManager*);
int StandardWidgetPaint(WidgetManager*);
int VolumeWidgetPaint(WidgetManager*);

#endif // WIDGET_H
