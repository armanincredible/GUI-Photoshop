#ifndef BRUSH_H
#define BRUSH_H

#include "../plugin.h"
//#include <QPixmap>

#include <SFML/Graphics.hpp>
#include <iostream>
#include "string.h"

class BrushWidget : public IWidget
{
public:
    Pair<int> start_point_ = {0, 0};
    Pair<int> size_ = {0, 0};

    void set_pos(Pair<int> point) override
    {
        start_point_ = point;
    }

    void set_size(Pair<int> point)
    {
        size_ = point;
    }

    Pair<int> get_size() override
    {
        return size_;
    }

    BrushWidget(Pair<int> point, Pair<int> point_size):
        start_point_(point)
    {
        set_size(point_size);
    }

    void on_mouse_press(Pair<int> /*point*/) override {
        std::cout << "Widget::on_mouse_press(Pair<int>)\n";
    }

    void on_mouse_release(Pair<int> /*point*/) override {
        std::cout << "Widget::on_mouse_release(Pair<int>)\n";
    }

    void on_mouse_move(Pair<int> /*point*/) override {
        std::cout << "Widget::on_mouse_move(Pair<int>)\n";
    }

    void on_key_press(int /*key*/) override {
        std::cout << "Widget::on_key_press(int)\n";
    }

    void on_key_release(int /*key*/) override {
        std::cout << "Widget::on_key_release(int)\n";
    }

    void draw(unsigned char* /*screen*/, int /*width*/, int /*height*/) override {
        std::cout << "Widget::draw(unsigned int*, int, int)\n";
    }

};

class BrushTool : public ITool
{
private:
    ColorPl<double> color_ = {};
    int thickness_ = 0;
public:
    void apply(unsigned char* pixmap, int width, int height, Pair<int> point) override
    {
        /*for (int i = 0; i < 10000; i++)
        {
            printf ("\n");
        }*/
        
        for (int j = point.y - thickness_; j < point.y + thickness_; j++)
        {
            for (int i = point.x - thickness_; i < point.x + thickness_; i++)
            {

                if ((i - point.x) * (i - point.x) + (j - point.y) * (j - point.y) <= thickness_ * thickness_)
                {
                    pixmap[j * 4 * width + i * 4] = color_.r * 255;
                    pixmap[j * 4 * width + i * 4 + 1] = color_.g * 255;
                    pixmap[j * 4 * width + i * 4 + 2] = color_.b * 255;
                    pixmap[j * 4 * width + i * 4 + 3] = 255;
                }
            }
            //fprintf (stderr, "end\n");
        }
        return;
    }

    void set_color(ColorPl<double> color) override
    {
        color_ = color;
        return;
    }

    ColorPl<double> get_color() override
    {
        return color_;
    }

    void set_thickness(int thickness) override
    {
        thickness_ = thickness;
        return;
    }

    int get_thickness() override
    {
        return thickness_;
    }

    void deactivate() override
    {
        return;
    }
};

class BrushButton : public IPushButton
{
public:
    Pair<int> start_point_ = {0, 0};
    Pair<int> size_ = {0, 0};
    const char* image_path_ = NULL;
    bool isContains_ = false;


    void set_pos(Pair<int> point) override
    {
        start_point_ = point;
    }

    void set_size(Pair<int> point)
    {
        size_ = point;
    }

    Pair<int> get_size() override
    {
        return size_;
    }

    BrushButton(Pair<int> point, Pair<int> point_size):
        start_point_(point)
    {
        set_size(point_size);
    }

    void contains(Pair<int> point) override {
        if (start_point_.x <= (unsigned) point.x && (unsigned) point.x <= (start_point_.x + size_.x)) {
            if (start_point_.y <= (unsigned) point.y && (unsigned) point.y <= (start_point_.y + size_.y)) {
                isContains_ = true;
                return;
            }
        }

        isContains_ = false;
    }

    Pair<int> get_pos() override {
        std::cout << "ToolButton::get_pos()\n";

        return Pair<int>{(int) start_point_.x, (int) start_point_.y};
    }

    void on_mouse_press(Pair<int> /*point*/) override {
        std::cout << "Widget::on_mouse_press(Pair<int>)\n";
    }

    void on_mouse_release(Pair<int> /*point*/) override {
        std::cout << "Widget::on_mouse_release(Pair<int>)\n";
    }

    void on_mouse_move(Pair<int> /*point*/) override {
        std::cout << "Widget::on_mouse_move(Pair<int>)\n";
    }

    void on_key_press(int /*key*/) override {
        std::cout << "Widget::on_key_press(int)\n";
    }

    void on_key_release(int /*key*/) override {
        std::cout << "Widget::on_key_release(int)\n";
    }

    void draw(unsigned char* screen, int width, int height) override {
        std::cout << "Widget::draw(unsigned int*, int, int)\n";
        //QPixmap pix (image_path_);
        //pix.
        fprintf (stderr, "in draw\n");
        if (image_path_)
        {
            int w = width;
            int h = height;

            sf::Image image;
            image.create(w, h);
            image.loadFromFile(image_path_);
            
            const sf::Uint8* pixels = image.getPixelsPtr();
            for (int j = 0; j < height; j++)
            {
                for (int i = 0; i < width; i++)
                {
                    screen[j * 4 * width + i * 4]     = pixels[j * 4 * width + i * 4];
                    screen[j * 4 * width + i * 4 + 1] = pixels[j * 4 * width + i * 4 + 1];
                    screen[j * 4 * width + i * 4 + 2] = pixels[j * 4 * width + i * 4 + 2];
                    screen[j * 4 * width + i * 4 + 3] = pixels[j * 4 * width + i * 4 + 3];
                }
                //fprintf (stderr, "end\n");
            }
        }
        else
        {
            unsigned x_ = 3;
            unsigned weight_ = size_.x;
            unsigned hight_ = size_.y;
            unsigned y_ = 3;
            unsigned outlineThickness_ = 3;


            for (int j = outlineThickness_; j < height - outlineThickness_; j++)
            {
                for (int i = outlineThickness_; i < width - outlineThickness_; i++)
                {
                    //fprintf (stderr, "%d\n", j * 3 * width + i);
                    screen[j * 3 * width + i * 3] = 255;
                    screen[j * 3 * width + i * 3 + 1] = 255;
                    screen[j * 3 * width + i * 3 + 2] = 255;
                }
                //fprintf (stderr, "end\n");
            }
        }
    }
    void set_signal(void (* /*signal*/)(IPushButton*)) override {
    }

};



#endif // BRUSH_H
