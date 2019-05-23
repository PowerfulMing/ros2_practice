/**
 * Project Alice
 */

#include "Track.h"
#include "jsoncpp/json/json.h"

Track::Track():BasicMapElement()
{
    start_point_radius_ = 5;
    end_point_radius_ = 5;
    start_point_fill_color_   = "red";
    end_point_fill_color_     = "black";
    start_point_border_color_ = "black";
    end_point_border_color_   = "red";
    // line
    track_width_ = 2;
    track_color_ = "blue";
    // interaction
    mouse_on_ = false;
}
Track::Track(int i,double x,double y,double z,double x2,double y2,double z2):BasicMapElement(i)
{
    set_start_point(x,y,z);
    set_end_point(x2,y2,z2);
    set_id(i);
    start_point_radius_ = 5;
    end_point_radius_ = 5;
    start_point_fill_color_   = "red";
    end_point_fill_color_     = "black";
    start_point_border_color_ = "black";
    end_point_border_color_   = "red";
    // line
    track_width_ = 2;
    track_color_ = "blue";
    // interaction
    mouse_on_ = false;

}
void Track::set_start_point(Location l)
{
    start_point_.set(l);
}
void Track::set_end_point(Location l)
{
    end_point_.set(l);
}
void Track::set_start_point(double x,double y)
{
    start_point_.set(x,y,0);
}
void Track::set_end_point(double x,double y)
{
    end_point_.set(x,y,0);
}

void Track::set_start_point(double x, double y, double z)
{
    start_point_.set(x,y,z);
}

void Track::set_end_point(double x, double y, double z)
{
    end_point_.set(x,y,z);
}
