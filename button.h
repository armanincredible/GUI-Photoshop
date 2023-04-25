#ifndef BUTTON_H
#define BUTTON_H
#include "vectors.h"
#include "tool.h"
#include "coordinate_system.h"
#include "layer_object.h"

enum FormType
{
    triangle,
    rectangle
};

class AbstractButton : public LayerObject
{
private:
    const char* name_ = NULL;
    Color color_ = {};
    FormType form_type_ = rectangle;

public:
    AbstractButton(char* name, Color color, Point a, Point b,  FormType form_type, Layer* layer):
        LayerObject(a, b, layer),
        name_(name),
        color_(color),
        form_type_(form_type)
    {}
    AbstractButton(Point a, Point b, Layer* layer):
        LayerObject(a, b, layer)
    {}

    void set_name(const char* name){name_ = name;}
    const char* get_name() const {return name_;}
};


class WidgetManager;


class Button : public AbstractButton
{
private:
    WidgetManager* my_widget_ = nullptr;
    Tool* my_tool_ = nullptr;
    const char* image_path_ = NULL;
    Color color_{};
    bool is_colored_ = false;

public:
    Button(char* name, Color color, Point start, Point end, int (*response)(Button*, WidgetManager*), Layer* layer):
        AbstractButton(name, color, start, end, FormType{rectangle}, layer)
    {
        response_ = response;
    }
    Button(char* name, Color color, Point start, Point end,  FormType form_type, Layer* layer):
        AbstractButton(name, color, start, end, FormType{rectangle}, layer)
    {}
    Button(Point start, Point end, Tool* tool,
           int (*response)(Button*, WidgetManager*),
           int (*paint_function)(Button*),
           Layer* layer):
        AbstractButton(start, end, layer),
        my_tool_(tool),
        response_(response),
        paint_function_(paint_function)
    {}
    Button(Point start, Point end,
           int (*response)(Button*, WidgetManager*),
           int (*paint_function)(Button*),
           Layer* layer):
        Button(start, end, nullptr, response, paint_function, layer)
    {}

    int (*paint_function_)(Button*) = NULL;
    int (*response_)(Button*, WidgetManager*);

    void set_tool(Tool* tool){my_tool_ = tool;}
    Tool* get_tool() const {return my_tool_;}

    void set_widget(WidgetManager* widget){my_widget_ = widget;}
    WidgetManager* get_widget() const {return my_widget_;}

    void set_image_path(const char* path){image_path_ = path;}
    const char* get_image_path(){return image_path_;}

    void set_color(Color color){color_ = color; is_colored_ = true;}
    Color get_color(){return color_;}
    bool is_colored(){return is_colored_;}
};

int button_with_instrument (Button*, WidgetManager*);
int button_change_volume (Button*, WidgetManager*);
int button_change_color_tool (Button*, WidgetManager*);
int button_change_thickness (Button*, WidgetManager*);

int StandardButtonPaint (Button*);
int ButtonPaintFromPicture (Button*);

#endif // BUTTON_H
