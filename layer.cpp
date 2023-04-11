#include "layer.h"
#include "window.h"
#include "widget.h"
#include "error.h"


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
