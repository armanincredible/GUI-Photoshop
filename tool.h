#ifndef TOOL_H
#define TOOL_H
#include "vectors.h"
#include <cstddef>
#include <cstdio>
#include <cstdlib>

class WidgetManager;

class Tool
{
private:
    Point last_click_ = {};
    int cur_num_click_ = 0;
    void* my_tool_manager_ = NULL;
    Color color_{};
    int thickness_ = 4;
public:
    int (*activity_)(Tool*, WidgetManager*, Point);

    Tool(int (*activity) (Tool*, WidgetManager*, Point)):
        activity_(activity)
    {};

    Point get_last_click (){return last_click_;}
    void set_last_click (Point click){last_click_ = click;}

    int get_cur_num_click (){return cur_num_click_;};
    void set_cur_num_click (int num){cur_num_click_ = num;}
    void add_cur_num_click (){cur_num_click_++;}

    void* get_tool_manager (){return my_tool_manager_;}
    void set_tool_manager (void* val){my_tool_manager_ = val;}
    int set_active_tool_in_manager ();

    void set_color(Color color){color_ = color;}
    Color get_color(){return color_;}

    void set_thickness(int val){thickness_ = val;}
    int get_thickness(){return thickness_;}
};

class ToolManager
{
private:
    Tool* cur_work_tool_ = NULL;
    Tool** tools_ = NULL;
    size_t tools_num_ = 0;
public:
    int add_tool (Tool* tool)
    {
        if (!tools_num_ && tools_)
        {
            printf ("ERROR %d\n", __LINE__);
        }
        if (!tool)
        {
            return -1;
        }
        tools_num_++;
        tools_ = (Tool**) realloc (tools_, tools_num_ * sizeof(Tool*));
        if (tools_ == NULL)
        {
            return -1;
        }
        tools_[tools_num_ - 1] = tool;
        tool->set_tool_manager(this);
        return 0;
    }
    Tool* get_active_tool (){return cur_work_tool_;}
    void set_active_tool (Tool* tool){cur_work_tool_ = tool;}

    ~ToolManager()
    {
        if (tools_)
        {
            free(tools_);
        }
    }
};


int paint_dot (Tool*, WidgetManager*, Point);
int paint_line (Tool*, WidgetManager*, Point);
int clear_dot (Tool*, WidgetManager*, Point);

#endif // TOOL_H
