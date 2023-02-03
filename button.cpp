#include "button.h"
#include "widget.h"
#include "error.h"
//#include "texteditor.h"

int button_with_instrument (Button* my_button, WidgetManager* obj)
{
    START_;
    Tool* tool = my_button->get_tool();
    if (tool == NULL)
    {
        PRINT_("button's tool null ptr\n");
        END_(-1);
    }
    if (tool->set_active_tool_in_manager())
    {
        PRINT_("button's tool cant become active\n");
        END_(-1);
    }
    else
    {
        WidgetManager* widget = (WidgetManager*) obj;
        widget->set_active_tool_manager((ToolManager*)tool->get_tool_manager());

        PRINT_("im here\n");

        widget->repaint_all_with_state(CurrentWork::ChangeActiveTool);

        END_(0);
    }
    END_(0);
}

int button_change_color_tool (Button* my_button, WidgetManager* obj)
{
    START_;
    WidgetManager* wid = (WidgetManager*)my_button->get_widget();
    if (wid != obj)
    {
        PRINT_("button's widget and his real widget controller are not equal\n");
        END_(-1);
    }

    Tool* tool = wid->get_active_tool_from_tool_manager();
    if (tool)
    {
        tool->set_color(my_button->get_color());

        WidgetManager* widget = (WidgetManager*) obj;
        widget->repaint_all_with_state(CurrentWork::ChangeActiveTool);
    }
    else
    {
        PRINT_("hasnt activity tool\n");
        END_(-1);
    }
    END_(0);
}

int button_change_thickness (Button* my_button, WidgetManager* obj)
{
    START_;
    WidgetManager* wid = (WidgetManager*)my_button->get_widget();
    if (wid != obj)
    {
        PRINT_("button's widget and his real widget controller are not equal\n");
        END_(-1);
    }

    Tool* tool = wid->get_active_tool_from_tool_manager();
    if (tool)
    {
        WidgetManager* widget = (WidgetManager*) obj;
        tool->set_thickness(4);

        widget->repaint_all_with_state(CurrentWork::ChangeActiveTool);
    }
    else
    {
        PRINT_("hasnt activity tool\n");
        END_(-1);
    }
    END_(0);
}

int StandardButtonPaint (Button* button)
{
    START_;
    if (button->is_colored())
    {
        Color color = button->get_color();
        button->get_layer()->paint_rectangle_with_area(button, button->get_widget(), color);
        END_(0);
    }
    button->get_layer()->paint_rectangle(button, button->get_widget());
    END_(0);
}

int ButtonPaintFromPicture (Button* button)
{
    START_;

    button->get_layer()->paint_rectangle(button, button->get_widget());
    
    if (button->get_image_path())
    {
        Point start {button->get_start_point()};
        Point end {button->get_end_point()};

        int w = end.x - start.x;
        int h = end.y - start.y;

        sf::Texture texture;
        if (!texture.loadFromFile(button->get_image_path()))
        {
            sf::Text text;
            text.setString(button->get_image_path());
            text.setPosition(sf::Vector2f(start.x, start.y));

            button->get_widget()->get_main_widget_()->draw(text);
            //painter->drawText(target, button->get_image_path(), Qt::AlignHCenter | Qt::AlignVCenter);

            PRINT_("Pixmap is emty because file image doesnt exit\n");
            END_(0);
        }
        else
        {
            sf::Sprite sprite;
            sprite.setTexture(texture);
            sf::Vector2u size = texture.getSize();
            sprite.setScale((float)w/size.x, (float)h/size.y);
            sprite.setPosition(start.x, start.y);
            button->get_widget()->get_main_widget_()->draw(sprite);
        }
        

        /*QRect target(start.x, start.y,
                     w, h);

        QPixmap pix (button->get_image_path());

        if (pix.isNull())
        {
            painter->drawText(target, button->get_image_path(), Qt::AlignHCenter | Qt::AlignVCenter);
            PRINT_("Pixmap is emty because file image doesnt exit\n");
            END_(0);
        }

        QRect source(0, 0, (pix.size()).width(), (pix.size()).height());
        pix.scaled(w, h, Qt::IgnoreAspectRatio);

        painter->drawPixmap(target, pix, source);
        button->get_layer()->paint_rectangle(button, painter);
        END_(0);*/
    }
    else
    {
        PRINT_("buttons image path is NULL\n");
        END_(0);
    }

    END_(-1);
}
