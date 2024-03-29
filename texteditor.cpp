﻿#include "texteditor.h"
#include "error.h"
#include <stdlib.h>
#include <string.h>

int StandardTextEditorPaint(WidgetManager* widget)
{
    START_;

    if (widget)
    {
        TextEditor* text_editor = (TextEditor*) widget;
        StandardWidgetPaint(text_editor);

        Tool* tool = widget->get_active_tool_from_tool_manager();
        if (tool && (widget->get_work_state() == CurrentWork::ChangeActiveTool))
        {
            text_editor->delete_all_data();
            InfoType type = text_editor->get_info_type();
            int val = 0;
            if (type == InfoType::RedColor)
            {
                val = tool->get_color().r * 100;
            }
            if (type == InfoType::BlueColor)
            {
                val = tool->get_color().b * 100;
            }
            if (type == InfoType::GreenColor)
            {
                val = tool->get_color().g * 100;
            }
            if (type == InfoType::Thickness)
            {
                val = tool->get_thickness();
            }
            char data[4] = "";
            sprintf(data, "%d", val);

            if (text_editor->get_data_len() <= 4)
            {
                text_editor->realloc_data(10);
            }
            strcpy(text_editor->get_data(), data);
        }

        int x0 = text_editor->get_start_point().x;
        int y0 = text_editor->get_start_point().y;
        int height = text_editor->CoordinateSystem::heigh();
        int widtht = text_editor->CoordinateSystem::width();

        widget->get_layer()->paint_rectangle_with_area(widget, {1, 1, 1});

        text_editor->get_layer()->paint_text(text_editor, text_editor->get_data());

        if ((widget->get_work_state() == CurrentWork::TimerReaction) && (widget->get_active_widget() == text_editor))
        {
            static bool line_need = true;

            if (line_need)
            {
                int y0 = text_editor->get_start_point().y;
                int x1 = text_editor->get_end_point().x - 2;
                int y1 = text_editor->get_end_point().y;

                widget->get_layer()->paint_line(widget, {(double)x1, (double)y0}, {(double)x1, (double)y1}, 3);

                line_need = false;
            }
            else
            {
                line_need = true;
            }
        }

        END_(0);
    }
    else
    {
        END_(-1);
    }
}

static void clear_stdin()
{
    while (getchar() != '\n');
}

int TextEditor::delete_data()
{
    START_;
    if (cur_symb_ != 0)
    {
        data_[--cur_symb_] = ' ';
    }

    get_main_widget_()->repaint_widget();

    END_(0);
}

int TextEditor::get_num()
{
    return atoi(get_data());
}

int TextEditor::delete_all_data()
{
    START_;
    while (cur_symb_ != 0)
    {
        data_[--cur_symb_] = ' ';
    }

    END_(0);
}

int TextEditor::realloc_data(int new_size)
{
    START_;
    if (data_ == NULL)
    {
        END_(-1);
    }

    data_ = (char*) realloc(data_, new_size);
    CHECK_NULL_PTR_(data_, "realloc is failed", -1);

    END_(0);
}


int TextEditor::set_data(const int symb)
{
    START_;
    if (data_ == NULL)
    {
        END_(-1);
    }

    if (cur_symb_ > data_len_ && data_len_ > 0)
    {
        data_ = (char*) realloc(data_, data_len_ * 2);
        CHECK_NULL_PTR_(data_, "realloc is failed", -1);
    }

    data_[cur_symb_] = symb;
    cur_symb_++;

    get_main_widget_()->repaint_widget();

    END_(0);
}

//add variable into text_editor which it contolls

int controller_text_editor(Button* button, WidgetManager* widget)
{
    START_;
    if (button)
    {
        button->response_(button, widget);
        END_(0);
    }
    if (widget)
    {
        TextEditor* text_editor = (TextEditor*) widget;
        Tool* tool = widget->get_active_tool_from_tool_manager();

        sf::Event::KeyEvent* key_event = text_editor->get_key_event();

        if (!key_event)
        {
            END_(0);
        }

        if (key_event->code == sf::Keyboard::Key::BackSpace)
        {
            text_editor->delete_data();
        }
        else if (key_event->code == sf::Keyboard::Key::Enter)
        {
            if (tool)
            {
                InfoType type = text_editor->get_info_type();
                if (type == InfoType::Thickness)
                {
                    int thickness = text_editor->get_num();
                    tool->set_thickness(thickness);
                }
                else
                {
                    Color color = tool->get_color();
                    int new_color = text_editor->get_num();
                    PRINT_("new_color %d\n", new_color);

                    if (type == InfoType::RedColor)
                    {
                        color.r = (double) new_color / 100;
                    }
                    if (type == InfoType::BlueColor)
                    {
                        color.b = (double) new_color / 100;
                    }
                    if (type == InfoType::GreenColor)
                    {
                        color.g = (double) new_color / 100;
                    }
                    tool->set_color(color);
                }
            }

            (text_editor->get_main_widget_())->repaint_widget();

            text_editor->set_active_widget(NULL);
        }
        else
        {
            //fprintf (stderr, "know i add %c", key_event->code + 'a');
            if (key_event->code >= sf::Keyboard::Num0 && key_event->code <= sf::Keyboard::Num9)
            {
                text_editor->set_data(key_event->code - sf::Keyboard::Num0 + '0');
            }
            //text_editor->set_data(key_event->code);
        }

        text_editor->set_key_event(NULL);
    }
    END_(0);
}
#include <cassert>
int timer_controller_text_editor(WidgetManager* widget)
{
    START_;
    //assert(0);
    static bool paint_line = true;
    if (widget == widget->get_active_widget())
    {
        (widget->get_main_widget_())->repaint_widget();
    }
    END_(0);
}

int last_activity_text_editor(WidgetManager* widget)
{
    START_;
    (widget->get_main_widget_())->repaint_widget();
    END_(0);
}

