#include "layer.h"
#include "window.h"
#include "widget.h"
#include "error.h"
#include "stack.h"


int Layer::paint_rectangle(LayerObject* object)
{
    START_;
    paint_rectangle_with_area(object, {0, 0, 0}, false);
    END_(0);
}

int Layer::paint_dot(LayerObject*, Point click, int thickness, Color color)
{
    START_;

    sf::CircleShape shape(thickness);

    shape.setPosition(sf::Vector2f(click.x - shape.getRadius(), click.y - shape.getRadius()));
    //shape.setOrigin(sf::Vector2f(click.x, click.y));
    shape.setFillColor(sf::Color(color.r * 255, color.g * 255, color.b * 255));

    get_canvas()->get_render_widget()->draw(shape);


    END_(0);
}

int Layer::paint_rectangle_with_area(LayerObject* object, Color color, bool area, Color outline_color)
{
    START_;

    int x0 = object->get_start_point().x;
    int y0 = object->get_start_point().y;

    int height = object->heigh();
    int widtht = object->width();

    sf::RectangleShape rectangle(sf::Vector2f(widtht, height));
    rectangle.setPosition(sf::Vector2f(x0, y0));

    //rectangle.setFillColor(sf::Color(100, 250, 50));
    rectangle.setOutlineColor(sf::Color(outline_color.r * 255, outline_color.g * 255, outline_color.b * 255));
    rectangle.setFillColor(sf::Color::Transparent);
    rectangle.setOutlineThickness(1);

    if (area)
    {
        rectangle.setFillColor(sf::Color(color.r * 255, color.g * 255, color.b * 255));
    }

    get_canvas()->get_render_widget()->draw(rectangle);

    END_(0);
}

int Layer::paint_text(LayerObject* obj, const char* str, bool scale)
{
    START_;

    Point start {obj->get_start_point()};
    Point end {obj->get_end_point()};

    int w = end.x - start.x;
    int h = end.y - start.y;

    sf::Font font;
    sf::Text text;
    
    font.loadFromFile("/usr/share/fonts/fonts-go/Go-Bold.ttf");
    text.setCharacterSize(h);
    text.setFont(font);
    text.setFillColor(sf::Color::Black);
    text.setString(str);

    sf::FloatRect textRect = text.getLocalBounds();
    text.setOrigin(textRect.left + textRect.width/2.0f,
                textRect.top  + textRect.height/2.0f);
    text.setPosition(sf::Vector2f(start.x + w/2.0f,start.y + h/2.0f));

    text.setLineSpacing(0);

    if (scale)
    {
        text.setScale((float)w / (text.getLocalBounds().width), (float)h / (text.getLocalBounds().height));
    }

    //text.

    get_canvas()->get_render_widget()->draw(text);

    END_(0);
}

int Layer::paint_image(LayerObject* obj, const char* str)
{
    START_;

    Point start {obj->get_start_point()};
    Point end {obj->get_end_point()};

    int w = end.x - start.x;
    int h = end.y - start.y;

    sf::Texture texture;
    texture.loadFromFile(str);
    sf::Sprite sprite;
    sprite.setTexture(texture);
    sf::Vector2u size = texture.getSize();
    sprite.setScale((float)w/size.x, (float)h/size.y);
    sprite.setPosition(start.x, start.y);
    get_canvas()->get_render_widget()->draw(sprite);

    END_(0);
}

static int push_into_stack_bit_ptr(char* pointer, char* start, char* end, Stack* st, IColor<char>* color_border, Color& into)
{
    START_;
    IColor<char>* cur_color = (IColor<char>*) pointer;
    if (pointer < end && pointer > start)
    {
        if (*cur_color != *color_border && 
            !(cur_color->r == (int)into.r && cur_color->g == (int)into.g && cur_color->b == (int)into.b))
        {
            PRINT_("push it\n");
            stack_push(st, pointer);
            cur_color->r = into.r;
            cur_color->g = into.g;
            cur_color->b = into.b;
        }
        END_(0);
    }
    END_(-1);
    
}

static int recursive_pour_region(char* bits, unsigned offset_click, unsigned w, unsigned h, Color from, Color into)
{
    START_;

    if (!bits)
    {
        PRINT_("bits null ptr\n");
        END_(-1);
    }

    char* start_bits = bits;
    char* end_bits = bits + h * w * 4;

    bool found_not_similar = false;
    IColor<char> border_color = {};
    Stack stack{};
    stack_ctor(&stack, 50); // TODO CHANGE SIZE
    char* cur_bits = bits + offset_click;

    PRINT_("color_from = %d %d %d\n", (int)from.r, (int)from.g, (int)from.b);
    PRINT_("color_into = %d %d %d\n", (int)into.r, (int)into.g, (int)into.b);

    *cur_bits   = into.r;
    cur_bits[1] = into.g; 
    cur_bits[2] = into.b;

    while(true)
    {
        PRINT_("color_cur = %d %d %d\n", *cur_bits, cur_bits[1], cur_bits[2]);

        if (!found_not_similar && !(*cur_bits == (int)from.r && cur_bits[1] == (int)from.g && cur_bits[2] == (int)from.b))
        {
            PRINT_("found border color\n ");
            border_color = {*cur_bits, cur_bits[1], cur_bits[2]};
            found_not_similar = true;
        }
        else
        {
            *cur_bits   = into.r;
            cur_bits[1] = into.g; 
            cur_bits[2] = into.b;
        }

        if (stack.size)
        {
            PRINT_("pop pointer\n ");
            stack_pop(&stack, &cur_bits);
        }

        int r = 0;
        r += push_into_stack_bit_ptr(cur_bits + 4, start_bits, end_bits, &stack, &border_color, into);
        r += push_into_stack_bit_ptr(cur_bits - 4, start_bits, end_bits, &stack, &border_color, into);
        r += push_into_stack_bit_ptr(cur_bits + w * 4, start_bits, end_bits, &stack, &border_color, into);
        r += push_into_stack_bit_ptr(cur_bits - w * 4, start_bits, end_bits, &stack, &border_color, into);

        if (!stack.size || r == -4)
        {
            break;
        }
    }

    END_(0);
}

int Layer::pour_region(LayerObject* obj, Point click, Color color_into)
{
    START_;

    //get_canvas()->get_render_widget()->add_button
    sf::Image snapshot = get_canvas()->get_render_widget()->capture();
    WidgetManager* MainWidget = get_canvas()->get_render_widget(); 
    const sf::Uint8* pixels = snapshot.getPixelsPtr();
    unsigned int pixelCount = snapshot.getSize().x * snapshot.getSize().y;

    Point start {obj->get_start_point()};
    Point end {obj->get_end_point()};

    int W = MainWidget->getSize().x;
    int H = MainWidget->getSize().y;
    int w = end.x - start.x;
    int h = end.y - start.y;

    char* array = obj->get_bit_array();
    int sym = 0;

    for (int y = 0; y < h; y++)
    {
        for (int x = 0; x < w; x++)
        {   
            array[y * w * 4 + x * 4]     = pixels[(y + (int)start.y) * W * 4 + (x + (int)start.x) * 4];
            array[y * w * 4 + x * 4 + 1] = pixels[(y + (int)start.y) * W * 4 + (x + (int)start.x) * 4 + 1];
            array[y * w * 4 + x * 4 + 2] = pixels[(y + (int)start.y) * W * 4 + (x + (int)start.x) * 4 + 2];
            array[y * w * 4 + x * 4 + 3] = pixels[(y + (int)start.y) * W * 4 + (x + (int)start.x) * 4 + 3];
        }
    }

    Color color_from = {(double)pixels[(int)click.y * W * 4 + (int)click.x * 4],
                        (double)pixels[(int)click.y * W * 4 + (int)click.x * 4 + 1],
                        (double)pixels[(int)click.y * W * 4 + (int)click.x * 4 + 2]};

    if (color_from != color_into)
    {
        PRINT_("colors are not similar\n");
        recursive_pour_region(array, (int)(click.y - start.y) * w * 4 + (int)(click.x - start.x) * 4, 
                              w, h, color_from, color_into);
    }


    /*In this part we draw our bit array*/

    sf::Image image;
    image.create(w, h, (const sf::Uint8*) array);

    // Create an SFML texture from the image
    sf::Texture texture;
    texture.loadFromImage(image);

    // Create an SFML sprite from the texture
    sf::Sprite sprite(texture);

    sprite.setPosition(start.x, start.y);
    MainWidget->draw(sprite);


    END_(0);
}

int Layer::paint_line(LayerObject* obj, Point prev_click, Point click, int thickness, Color color)
{
    START_;

    double len_x = click.x - prev_click.x;
    double len_y = click.y - prev_click.y;
    double len_square = len_x*len_x + len_y*len_y;
    double len = sqrt(len_square);

    double cosa = len_x * 1 / (len);
    double alpha = 0;
    if (len_y > 0)
    {
        alpha = acos(cosa)* 180 / M_PI;
    }
    else
    {
        alpha = - acos(cosa)* 180 / M_PI;
    }

    sf::RectangleShape line(sf::Vector2f(len, thickness));

    line.setFillColor(sf::Color(color.r * 255, color.g * 255, color.b * 255));

    line.rotate(alpha);
    line.setPosition(prev_click.x, prev_click.y);

    get_canvas()->get_render_widget()->draw(line);

    END_(0);
}

LayerObject* Layer::find_object_on_layer(Point xy)
{
    for (int i = 0; i < widgets_num_; i++)
    {
        if (widgets_[i]->is_my_area(xy))
        {
            return widgets_[i];
        }
    }
    return NULL;
}
