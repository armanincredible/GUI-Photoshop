#ifndef LAYER_H
#define LAYER_H

#include "vectors.h"
#include <cstdio>

class Canvas;

class LayerObject;

class WidgetManager;

class Layer
{
private:
    int level_ = 0;
    Canvas* canvas_ = NULL;
    LayerObject** widgets_ = NULL;
    size_t widgets_num_ = 0;

public:
    int add_object(LayerObject* obj)
    {
        if (!widgets_num_ && widgets_)
        {
            printf ("ERROR %d\n", __LINE__);
        }
        widgets_num_++;
        widgets_ = (LayerObject**) realloc (widgets_, widgets_num_ * sizeof(LayerObject*));
        if (widgets_ == NULL)
        {
            return -1;
        }
        widgets_[widgets_num_ - 1] = obj;
        return 0;
    }
    LayerObject** get_widgets(void){return widgets_;}

    void set_level(int lvl){level_ = lvl;}
    int get_level(){return level_;}

    void set_canvas(Canvas* canvas){canvas_ = canvas;}
    Canvas* get_canvas(void){return canvas_;}

    Layer(int lvl):
        level_(lvl)
    {};

    int paint_rectangle(LayerObject*);
    int paint_rectangle_with_area(LayerObject*, Color, bool = true, Color = {0, 0, 0});
    int paint_dot(LayerObject*, Point, int, Color = {0, 0, 0});
    int paint_line(LayerObject*, Point, Point, int, Color = {0, 0, 0});
};

class Canvas
{
private:
    WidgetManager* my_render_widget_ = NULL;
    Layer** layers_ = NULL;
    size_t layer_num_ = 0;
    Layer* active_layer_ = NULL;
public:
    //Canvas();
    WidgetManager* get_render_widget(){return my_render_widget_;}
    void set_render_widget(WidgetManager* widget){my_render_widget_ = widget;}

    int add_layer (Layer* layer)
    {
        if (!layer_num_ && layers_)
        {
            printf ("ERROR %d\n", __LINE__);
        }
        layer_num_++;
        layers_ = (Layer**) realloc (layers_, layer_num_ * sizeof(Layer*));
        if (layers_ == NULL)
        {
            return -1;
        }
        layers_[layer_num_ - 1] = layer;
        layer->set_canvas(this);
        return 0;
    }
    Layer* get_active_layer (){return active_layer_;}
    void set_active_layer (Layer* layer){active_layer_ = layer;}

};

#endif // LAYER_H
