#include "layer.h"
#include "coordinate_system.h"
#include "widget.h"
#include "error.h"
#include "stack.h"
#include "layer_object.h"
#include <thread>


int Layer::paint_rectangle(LayerObject* object)
{
    START_;
    paint_rectangle_with_area(object, {0, 0, 0}, false);
    END_(0);
}

int Layer::paint_dot(LayerObject* object, Point click, int thickness, Color color)
{
    START_;

    sf::CircleShape shape(thickness);

    shape.setPosition(sf::Vector2f(click.x - shape.getRadius(), click.y - shape.getRadius()));
    //shape.setOrigin(sf::Vector2f(click.x, click.y));
    shape.setFillColor(sf::Color(color.r * 255, color.g * 255, color.b * 255));

    get_canvas()->get_render_widget()->draw(shape);

    //object->fill_bits_from_widget_manager(get_canvas()->get_render_widget());
    //std::thread thr(&LayerObject::fill_bits_from_widget_manager, object, get_canvas()->get_render_widget());
    //thr.detach();

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

    //object->fill_bits_from_widget_manager(get_canvas()->get_render_widget());
    //std::thread thr(&LayerObject::fill_bits_from_widget_manager, object, get_canvas()->get_render_widget());
    //thr.detach();

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
    //obj->fill_bits_from_widget_manager(get_canvas()->get_render_widget());

    //std::thread thr(&LayerObject::fill_bits_from_widget_manager, obj, get_canvas()->get_render_widget());
    //thr.detach();

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

    //obj->fill_bits_from_widget_manager(get_canvas()->get_render_widget());
    //std::thread thr(&LayerObject::fill_bits_from_widget_manager, obj, get_canvas()->get_render_widget());
    //thr.detach();

    END_(0);
}

static int push_into_stack_bit_ptr(char* pointer, char* start, char* end, Stack* st, IColor<char>* from_color, Color& into)
{
    START_;
    IColor<char>* cur_color = (IColor<char>*) pointer;
    if (pointer < end && pointer > start)
    {
        if (*cur_color == *from_color && 
            !(cur_color->r == (int)into.r && cur_color->g == (int)into.g && cur_color->b == (int)into.b))
        {
            PRINT_("push it\n");
            stack_push(st, pointer);
            cur_color->r = (char)(into.r  * 255);
            cur_color->g = (char)(into.g * 255);
            cur_color->b = (char)(into.b * 255);
        }
        END_(0);
    }
    END_(-1);
    
}

static int recursive_pour_region(char* bits, unsigned offset_click, unsigned w, unsigned h, Color into)
{
    START_;

    if (!bits)
    {
        PRINT_("bits null ptr\n");
        END_(-1);
    }

    char* cur_bits = bits + offset_click;
    if (*cur_bits == into.r * 255 && cur_bits[1] == into.g * 255 && cur_bits[2] == into.b * 255)
    {
        PRINT_("colors are similar\n");
        END_(0);
    }

    char* start_bits = bits;
    char* end_bits = bits + h * w * 4;

    IColor<char> from_color = {};
    Stack stack{};
    stack_ctor(&stack, 50); // TODO CHANGE SIZE

    PRINT_("color_into = %d %d %d\n", (int)into.r, (int)into.g, (int)into.b);

    from_color = {*cur_bits, cur_bits[1], cur_bits[2]};
    //border_color = {(char)from.r, (char)from.g, (char)from.b};
    *cur_bits   = (char)(into.r * 255);
    cur_bits[1] = (char)(into.g * 255); 
    cur_bits[2] = (char)(into.b * 255);

    while(true)
    {
        PRINT_("color_cur = %d %d %d\n", *cur_bits, cur_bits[1], cur_bits[2]);

        if (stack.size)
        {
            PRINT_("pop pointer\n ");
            stack_pop(&stack, &cur_bits);
        }

        int r = 0;
        r += push_into_stack_bit_ptr(cur_bits + 4, start_bits, end_bits, &stack, &from_color, into);
        r += push_into_stack_bit_ptr(cur_bits - 4, start_bits, end_bits, &stack, &from_color, into);
        r += push_into_stack_bit_ptr(cur_bits + w * 4, start_bits, end_bits, &stack, &from_color, into);
        r += push_into_stack_bit_ptr(cur_bits - w * 4, start_bits, end_bits, &stack, &from_color, into);

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

    WidgetManager* MainWidget = get_canvas()->get_render_widget(); 

    Point start {obj->get_start_point()};
    Point end {obj->get_end_point()};

    int W = MainWidget->getSize().x;
    int H = MainWidget->getSize().y;
    int w = end.x - start.x;
    int h = end.y - start.y;
    char* array = obj->get_bit_array();


    obj->fill_bits_from_widget_manager(get_canvas()->get_render_widget());

    recursive_pour_region(array, (int)(click.y - start.y) * w * 4 + (int)(click.x - start.x) * 4, 
                        w, h, color_into);


    /*In this part we draw our bit array*/

    paint_from_bits(obj);

    END_(0);
}

int Layer::paint_from_bits(LayerObject* obj)
{
    START_;

    Point start {obj->get_start_point()};
    Point end {obj->get_end_point()};
    int w = end.x - start.x;
    int h = end.y - start.y;
    char* array = obj->get_bit_array();
    WidgetManager* MainWidget = get_canvas()->get_render_widget();

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

    //std::thread thr(&LayerObject::fill_bits_from_widget_manager, obj, get_canvas()->get_render_widget());
    //thr.detach();
    //obj->fill_bits_from_widget_manager(get_canvas()->get_render_widget());

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
