#ifndef ADAPTER_H
#define ADAPTER_H

#include "../widget.h"
#include "../plugin.h"
#include "../error.h"

#ifndef WIN32
#include <dlfcn.h>
#else
#include <windows.h>
#endif

int WidgetAdapterPaint(WidgetManager* widget, QPainter* painter);
WidgetManager* from_iwidget_to_widget(IWidget* widget);
int controller_adapter_widget (Button* button, WidgetManager* widget);
int ToolActivityAdapter(Tool* tool, QPainter* painter, Point p);

class ToolAdapter: public Tool
{
public:
    ITool* itool_ = NULL;
    WidgetManager* paint_widget_ = NULL;

    ToolAdapter(ITool* tool, WidgetManager* paint_widget):
        Tool(ToolActivityAdapter),
        itool_(tool),
        paint_widget_(paint_widget)
    {}
};



class WidgetAdapter : public WidgetManager
{
public:
    IWidget* iwidget_ = NULL;
    QImage src_ = {};
    WidgetAdapter(IWidget* iwidget, WidgetManager* parent, Layer* layer):
        WidgetManager({(double)iwidget->get_pos().x, (double)iwidget->get_pos().y},
                      {(double)iwidget->get_pos().x + iwidget->get_size().x,
                       (double)iwidget->get_pos().y + iwidget->get_size().y},
                       parent, controller_adapter_widget, WidgetAdapterPaint, layer),
        iwidget_(iwidget)
    {}
};

int ButtonAdapterController (Button*, WidgetManager*);

int ButtonAdapterPaint (Button*, QPainter*);

class ButtonAdapter: public Button
{
public:
    IPushButton* ibutton_ = NULL;

    ButtonAdapter(IPushButton* ibutton, Layer* layer):
        Button({(double)ibutton->get_pos().x, (double)ibutton->get_pos().y},
              {(double)ibutton->get_pos().x + ibutton->get_size().x,
               (double)ibutton->get_pos().y + ibutton->get_size().y},
                ButtonAdapterController, ButtonAdapterPaint, layer),
        ibutton_(ibutton)
    {}
};

class PluginAdapter
{
public:
    IPlugin* iplugin_ = NULL;

    IPlugin* (*get_plugin)() = NULL;
    void* lib_ = NULL;
    void (*destroy_plugin)() = NULL;

    WidgetManager* paint_widget_ = NULL;

    Tool* tool_ = NULL;
    WidgetManager* props_ = NULL;
    Button* tool_button_ = NULL;

    Tool* make_tool_from_itool(ITool* itool, WidgetManager* paint_widget)
    {
        START_;
        if (!itool)
        {
            PRINT_("Plugin hasnt tool\n");
            END_(NULL);
        }
        ToolAdapter* tool_adapter = new ToolAdapter(itool, paint_widget);
        END_(tool_adapter);
    }

    WidgetManager* make_widget_from_iwidget(IWidget* iwidget, WidgetManager* parent_widget, Layer* layer)
    {
        START_;
        if (!iwidget)
        {
            PRINT_("Plugin hasnt props widget\n");
            END_(NULL);
        }
        WidgetAdapter* widget_adapter = new WidgetAdapter(iwidget, parent_widget, layer);
        END_(widget_adapter);
    }

    Button* make_button_from_ibutton(IPushButton* ibutton, Layer* layer)
    {
        START_;
        if (!ibutton)
        {
            PRINT_("Plugin hasnt press button\n");
            END_(NULL);
        }
        ButtonAdapter* button_adapter = new ButtonAdapter(ibutton, layer);
        END_(button_adapter);
    }

    int load_plugin(char* lib_name)
    {
        START_;
        #ifndef WIN32
           lib_ = dlopen(lib_name, RTLD_LAZY);
        #else
           lib_ = LoadLibrary(s);
        #endif
           if (!lib_) {
             printf("cannot open library '%s'\n", lib_name);
             return -1;
           }
        #ifndef WIN32
           get_plugin = (IPlugin* (*)())dlsym(lib_, "get_plugin");
           destroy_plugin = (void (*)()) dlsym(lib_, "destroy_plugin");
        #else
           get_plugin = (IPlugin* (*)())GetProcAddress((HINSTANCE)lib_, "get_plugin");
           destroy_plugin = (void (*)())GetProcAddress((HINSTANCE)lib_, "destroy_plugin");
        #endif
           if (get_plugin == NULL || destroy_plugin == NULL) {
             printf("cannot load function\n");
             return -1;
           }

           iplugin_ = get_plugin();
           END_(0);
    }

    void delete_plugin()
    {
        if (destroy_plugin)
        {
            destroy_plugin();
        }
        if (lib_)
        {
            #ifndef WIN32
               dlclose(lib_);
            #else
               FreeLibrary((HINSTANCE)lib_);
            #endif
        }
    }


    PluginAdapter(char* path, WidgetManager* paint_wiget, WidgetManager* parent_widget, Layer* layer)
    {
        if (!load_plugin(path))
        {
            paint_widget_ = paint_wiget;

            if (iplugin_)
            {
                tool_ = make_tool_from_itool(iplugin_->get_tool(), paint_wiget);
                props_ = make_widget_from_iwidget(iplugin_->get_props(), parent_widget, layer);
                tool_button_ = make_button_from_ibutton(iplugin_->get_tool_button(), layer);
                tool_button_->set_tool(tool_);

                if (tool_button_ && tool_)
                {
                    tool_button_->set_tool(tool_);
                }
            }
        }
    }
};

/*
class PluginManager
{
    PluginAdapter** plugins_ = NULL;
    int plugins_num = 0;

    int add_plugin(PluginAdapter* plugin)
    {

    }
};*/

#endif // ADAPTER_H
