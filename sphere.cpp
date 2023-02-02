#include "sphere.h"
#include "math.h"
#include <unistd.h>
#include "square_equation.h"
#include "control.h"

static const int MAX_NUM_REFLECTION = 3;
static const double Ambient = 0.1;

static int is_eq (double num1, double num2)
{
    return (fabs(num1 - num2) < EPS);
}

static double hypotenuse_sqaure (double num1, double num2)
{
    return square(num1) + square(num2);
}

static int hypotenuse_sqaure (int num1, int num2)
{
    return square(num1) + square(num2);
}

static double calculate_z (const double r, const double x, const double y)
{
    return sqrt (square(r) - square(x) - square(y));
}

void Window_Sphere::paintEvent(QPaintEvent *)
{
    QPainter painter(this);

    QRect target(0, 0, size_.x, size_.y);

#ifdef ONLY_SPHERE
    paintSphere();
#endif

#ifdef RACING
    paintRacing();
#endif

    QPixmap pmap(size_.x, size_.y);
    pmap.convertFromImage(src_);

    painter.drawPixmap(target, pmap, target);
}

void Window_Sphere::paintSphere()
{
    uchar* collor_buffer = src_.bits();
    Lamp lamp = lamps_.front();
    Sphere sphere = spheres_.front();
    double radius_2 = pow(sphere.get_radius(), 2);
    double origin_x = get_origin_point().x;
    double origin_y = get_origin_point().y;
    Vector ray_and_material_collor = sphere.get_color_material() ^ lamp.get_collor_ray();

    /*double r = 150;
    static double alpha = 0;
    double x = r * cos(alpha);
    double y = r * sin(alpha);
    set_viewer_point (Point_int{(int)x, (int)y, 500});

    double x_ = r * cos(alpha + 3.14 * 1/2);
    double y_ = r * sin(alpha + 3.14 * 1/2);
    set_lamp_point (Point_int{(int)x_, (int)y_, 500});*/

    for (int y_len = 0; y_len < size_.y; y_len++)
    {
        for (int x_len = 0; x_len < size_.x; x_len++)
        {
            Point point_on_screen {x_len - origin_x, origin_y - y_len};

            if (hypotenuse_sqaure(point_on_screen.x, point_on_screen.y) <= radius_2)
            {
                point_on_screen.z = calculate_z (sphere.get_radius(), point_on_screen.x, point_on_screen.y);

                Vector Color = ray_and_material_collor * (calculate_diffuse(point_on_screen, sphere, lamp) + Ambient) +
                                lamp.get_collor_ray() * pow(calculate_specular(point_on_screen, sphere, lamp), sphere.get_power_of_specular());
                Point Color_point = Color.get_in_point();

                clap_value(Color_point.x);
                clap_value(Color_point.y);
                clap_value(Color_point.z);
                collor_buffer[y_len * size_.x * 3 + 2 + x_len * 3] = (uchar)(Color_point.z * 250);
                collor_buffer[y_len * size_.x * 3 + 1 + x_len * 3] = (uchar)(Color_point.y * 250);
                collor_buffer[y_len * size_.x * 3 + 0 + x_len * 3] = (uchar)(Color_point.x * 250);
            }
        }
    }

    //alpha += 3.14 / 10;
}

void Window_Sphere::paintCircle(Sphere& sphere)
{
    uchar* collor_buffer = src_.bits();
    double radius_2 = pow(sphere.get_radius(), 2);
    double origin_x = get_origin_point().x;
    double origin_y = get_origin_point().y;
    Vector material_collor = sphere.get_color_material();

    for (int y_len = 0; y_len < size_.y; y_len++)
    {
        for (int x_len = 0; x_len < size_.x; x_len++)
        {
            Point center_sphere = sphere.get_center_coordinate();
            Point point_on_screen {x_len - origin_x - center_sphere.x, origin_y - y_len- center_sphere.y};

            if (hypotenuse_sqaure(point_on_screen.x, point_on_screen.y) <= radius_2)
            {
                point_on_screen.z = calculate_z (sphere.get_radius(), point_on_screen.x, point_on_screen.y);

                Point Color_point = material_collor.get_in_point();

                clap_value(Color_point.x);
                clap_value(Color_point.y);
                clap_value(Color_point.z);
                collor_buffer[y_len * size_.x * 3 + 2 + x_len * 3] = (uchar)(Color_point.z * 250 * point_on_screen.z / sphere.get_radius());
                collor_buffer[y_len * size_.x * 3 + 1 + x_len * 3] = (uchar)(Color_point.y * 250 * point_on_screen.z / sphere.get_radius());
                collor_buffer[y_len * size_.x * 3 + 0 + x_len * 3] = (uchar)(Color_point.x * 250 * point_on_screen.z / sphere.get_radius());
            }
        }
    }
}

void Window_Sphere::paintRacing()// // // // // // // / / // // / /// / / // / / / // / ////// ///////// // // // // / / / // / /
{
    uchar* collor_buffer = src_.bits();
    double origin_x = get_origin_point().x;
    double origin_y = get_origin_point().y;

    for (int y_len = 0; y_len < size_.y; y_len++)
    {
        for (int x_len = 0; x_len < size_.x; x_len++)
        {
            Point point_on_screen {x_len - origin_x, origin_y - y_len};

            Vector v({(double)viewer_.x, (double)viewer_.y, (double)viewer_.z}, {point_on_screen.x, point_on_screen.y, 0});
            Point point_on_sphere = {};

            Vector Color = get_color_vector_on_screen (v);
            Point Color_point = Color.get_in_point();

            clap_color_point(Color_point);

            collor_buffer[y_len * size_.x * 3 + 2 + x_len * 3] = (uchar)(Color_point.z * 250);
            collor_buffer[y_len * size_.x * 3 + 1 + x_len * 3] = (uchar)(Color_point.y * 250);
            collor_buffer[y_len * size_.x * 3 + 0 + x_len * 3] = (uchar)(Color_point.x * 250);

        }
    }
    for (Lamp lamp : lamps_)
    {
        Sphere sphere(50, 0, lamp.get_lamp_point(), lamp.get_collor_ray().get_in_point(), 0);
        paintCircle(sphere);
    }
}

inline Vector Window_Sphere::get_color_vector_on_screen(Vector& v)
{
    Vector color({0, 0, 0}, {0, 0, 0});

    bool crossed_sphere = false;

    for (Sphere sphere : spheres_)
    {
        Point point_on_sphere = {};
        if (get_points_crossed_sphere(sphere, v, point_on_sphere))
        {
            continue;
        }

        crossed_sphere = true;
        int save_num_reflection = cur_num_reflection_;

        Point center = sphere.get_center_coordinate();
        Vector normal (center, {point_on_sphere.x, point_on_sphere.y, point_on_sphere.z});
        normal.NormalizeVector();
        v.NormalizeVector();
        Vector v_by_n = normal * 2 + v;

        for (int cur_num_reflection_ray = 0;
             cur_num_reflection_ray < sphere.get_num_reflections_rays();
             cur_num_reflection_ray++) //cur_num_reflection_ray num of rays of one sphere,
                                       //cur_num_reflection_ num of reflection
        {
            cur_num_reflection_++;
            if (cur_num_reflection_ >= max_num_reeflecions_ + 1)
            {
                color.change_points({0, 0, 0}, {0, 0.01, 0.08});
                cur_num_reflection_ = save_num_reflection;
                return color;
            }
            v_by_n.RotateOnRandomCorner();
            color += get_color_vector_on_screen(v_by_n) / (2 * sphere.get_num_reflections_rays());
            cur_num_reflection_ = save_num_reflection;
        }
        color += get_color_vector_on_sphere(sphere, point_on_sphere);
        break;
    }

#ifdef SHADOW

    if (crossed_sphere == false)
    {
        color += get_color_vector_env(v);
    }
#endif

    return color;
}

inline Vector Window_Sphere::get_color_vector_env(Vector &v)
{
    Vector color = {};
    Vector normal_plane ({0, 0, 0}, {0, 1, 0});

    if (normal_plane * v < 0)
    {
        color.change_points({0, 0, 0}, {0, 0.3, 0});

        Point v2 = v.get_in_point();
        Point v1 = v.get_out_point();

        double x = 0;
        double z = 0;
        double koef = 0;
        if (v1.y != v2.y)
        {
            koef = v1.y / (v1.y - v2.y);
        }
        x = koef * (v2.x - v1.x) + v1.x;
        z = koef * (v2.z - v1.z) + v1.z;

        Point cross_plane_by_n {x, 0, z};

        for (Lamp lamp : lamps_)
        {
            Point lamp_point = lamp.get_lamp_point();
            Vector to_place (cross_plane_by_n, lamp_point);
            for (Sphere sphere: spheres_)
            {
                Point center = sphere.get_center_coordinate();

                Point point_on_sphere = {};
                if (!get_points_crossed_sphere(sphere, to_place, point_on_sphere))
                {
                    color *= 1 / 1.2;
                    //color.change_points({0, 0, 0}, {0.5, 0, 0});
                }
            }
        }
    }
    else
    {
        color.change_points({0, 0, 0}, {0, 0, 0.1});
    }

    return color;
}

inline Vector Window_Sphere::get_color_vector_on_sphere(Sphere& sphere, Point& point_on_sphere)
{
    Vector color({0, 0, 0}, {0, 0, 0});
    for (Lamp lamp : lamps_)
    {
        Vector ray_and_material_collor = sphere.get_color_material() ^ lamp.get_collor_ray();
        color += ray_and_material_collor * (calculate_diffuse(point_on_sphere, sphere, lamp) + Ambient) +
                 lamp.get_collor_ray() * pow(calculate_specular(point_on_sphere, sphere, lamp), sphere.get_power_of_specular());
    }
    return color;
}

void Window_Sphere::clap_color_point(Point& color_point)
{
    clap_value(color_point.x);
    clap_value(color_point.y);
    clap_value(color_point.z);
}

inline int Window_Sphere::get_points_crossed_sphere (Sphere& sphere, Vector& v, Point& point)
{
    double radius = sphere.get_radius();
    Point center_point = sphere.get_center_coordinate();

    Vector shit (v.get_out_point(), center_point);
    if (shit * v <= 0)
    {
        return 1;
    }

    Vector a(center_point, v.get_out_point()); // AC
    Vector square = v | a;
    double length = sqrt(square.get_sqaure_length()) / sqrt(v.get_sqaure_length());

    if (length <= radius)
    {
        double t1 = 0;
        double t2 = 0;

        solve_quadratic_equation(v.get_sqaure_length(), 2 * (v * a), a.get_sqaure_length() - square(radius), &t1, &t2);
        Vector new_v = v * t1;

        point = new_v.get_in_point();

        return 0;
    }

    return 1;
}

double Window_Sphere::calculate_diffuse (Point point_on_sphere, Sphere& sphere, Lamp& lamp)
{
    Point center = sphere.get_center_coordinate();
    Vector vec_lamp (center, lamp.get_lamp_point());
    Vector normal (center, {point_on_sphere.x, point_on_sphere.y, point_on_sphere.z});

    double cos_fi = get_cos(vec_lamp, normal);
    if (cos_fi < 0)
    {
        cos_fi = 0;
    }

    //printf ("%lg ", cos_fi);
    return cos_fi;
}

double Window_Sphere::calculate_specular (Point point_on_sphere, Sphere& sphere, Lamp& lamp)
{
    Point center = sphere.get_center_coordinate();
    Vector l(lamp.get_lamp_point(), {point_on_sphere.x, point_on_sphere.y, point_on_sphere.z});
    l.NormalizeVector();

    Vector normal (center, {point_on_sphere.x, point_on_sphere.y, point_on_sphere.z});
    normal.NormalizeVector();

    Vector l_by_n = normal * 2 + l;
    Vector v({point_on_sphere.x, point_on_sphere.y, point_on_sphere.z}, {(double)viewer_.x, (double)viewer_.y, (double)viewer_.z});

    double cos_alpha = get_cos(l_by_n, v);

    if (cos_alpha < 0)
    {
        cos_alpha = 0;
    }
    //printf ("%lg ", cos_alpha);
    return cos_alpha;
}

