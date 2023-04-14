#pragma once

#include <SFML/Graphics.hpp>
#include "math.h"
#include "layer.h"

class LayerObject : public CoordinateSystem
{
private:
    Layer* layer_ = NULL;
    char* bit_array_ = NULL;
public:
    LayerObject(Point a, Point b, Layer* layer):
        CoordinateSystem(a, b)
    {
        bit_array_ = new char[((int)b.y - (int)a.y) * ((int)b.x - (int)a.x) * 4];//(char*) calloc ((b.y - a.y) * (b.x - a.x) * 4, 0);
        set_layer(layer);
    }
    char* get_bit_array(){return bit_array_;}
    
    int fill_bits_from_widget_manager(WidgetManager*);
    int set_layer(Layer* layer){layer_ = layer; return layer->add_object(this);}
    Layer* get_layer(void){return layer_;}
};