#ifndef BRUSH_H
#define BRUSH_H

#include "../plugin.h"
//#include <QPixmap>

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
    void apply(unsigned char* pixmap, int width, int height, Pair<int> point) override
    {
        /*for (int i = 0; i < 10000; i++)
        {
            printf ("\n");
        }*/
        for (int j = 0; j < height; j++)
        {
            for (int i = 0; i < width; i++)
            {
                //fprintf (stderr, "%d\n", j * 3 * width + i);
                pixmap[j * 3 * width + i * 3] = 255;
                pixmap[j * 3 * width + i * 3 + 1] = 255;
                pixmap[j * 3 * width + i * 3 + 2] = 255;
            }
            //fprintf (stderr, "end\n");
        }

        for (int j = point.y - 3; j < point.y + 3; j++)
        {
            for (int i = point.x - 3; i < point.x + 3; i++)
            {
                pixmap[j * 3 * width + i * 3] = 0;
                pixmap[j * 3 * width + i * 3 + 1] = 0;
                pixmap[j * 3 * width + i * 3 + 2] = 0;
            }
        }
        return;
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
            /*fprintf (stderr, "in draw image\n");
            QImage image(image_path_);
            if (image.isNull())
            {
                fprintf (stderr, "image is null\n");
                return;
            }
            image.scaled(width, height);
            //image.copy()

            image.convertTo(QImage::Format_RGB888);
            fprintf (stderr, "%d in bytes", (int)image.sizeInBytes());
            uchar* array = image.bits();

            //memcpy(screen, image.bits(), width * 3 * height);
            for (int j = 0; j < height; j++)
            {
                for (int i = 0; i < width; i++)
                {
                    //fprintf (stderr, "%d\n", j * 3 * width + i);
                    screen[j * 3 * width + i * 3] = array[j * 3 * width + i * 3];
                    screen[j * 3 * width + i * 3 + 1] = array[j * 3 * width + i * 3 + 1];
                    screen[j * 3 * width + i * 3 + 2] = array[j * 3 * width + i * 3 + 1];
                }
                //fprintf (stderr, "end\n");
            }
            //memcpy(screen, image.bits(), width * 3 * height);
            fprintf (stderr, "in draw2\n");*/
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
