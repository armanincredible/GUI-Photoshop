#include "layer_object.h"
#include "widget.h"
#include "error.h"

#define MIN(a, b) ((a) < (b) ? (a) : (b))

int LayerObject::fill_bits_from_widget_manager(WidgetManager* widget)
{
    START_;
    if (!widget)
        END_(-1);

    if (widget == widget->get_main_widget_())
    {
        PRINT_("im main widget\n");
    }

    sf::Image snapshot = widget->capture();
    const sf::Uint8* pixels = snapshot.getPixelsPtr();
    unsigned int pixelCount = snapshot.getSize().x * snapshot.getSize().y;

    Point start {get_start_point()};
    Point end   {get_end_point()};

    int W = widget->getSize().x;
    int H = widget->getSize().y;
    int w = end.x - start.x;
    int h = end.y - start.y;

    //printf("W H = %d %d\n", W, H);
    //printf("w h = %d %d\n", w, h);

    char* array = get_bit_array();
    if (!array)
    {
        PRINT_("array NULL\n");
    }

    for (int y = 0; y < MIN(h, H); y++)
    {
        for (int x = 0; x < w; x++)
        {   
            array[y * w * 4 + x * 4]     = pixels[(y + (int)start.y) * W * 4 + (x + (int)start.x) * 4];
            array[y * w * 4 + x * 4 + 1] = pixels[(y + (int)start.y) * W * 4 + (x + (int)start.x) * 4 + 1];
            array[y * w * 4 + x * 4 + 2] = pixels[(y + (int)start.y) * W * 4 + (x + (int)start.x) * 4 + 2];
            array[y * w * 4 + x * 4 + 3] = pixels[(y + (int)start.y) * W * 4 + (x + (int)start.x) * 4 + 3];
        }
    }

    END_(0);
}