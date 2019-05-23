/**
 * Project Alice
 */

#ifndef TRACK_H
#define TRACK_H
#include "Location.h"
#include "Basicmapelement.h"
#include <string>
using namespace std;
class Track : public BasicMapElement
{
private:
    Location start_point_,end_point_;

public:
    // ui related
        // point
        int start_point_radius_ = 5;
        int end_point_radius_ = 5;
        string start_point_fill_color_   = "red";
        string end_point_fill_color_     = "black";
        string start_point_border_color_ = "black";
        string end_point_border_color_   = "red";
        // line
        int track_width_ = 2;
        string track_color_ = "blue";
        // interaction
        bool mouse_on_ = false;
    Track();
    Track(int,double,double,double,double,double,double);

    void set_start_point(Location);
    void set_end_point(Location);
    void set_start_point(double,double);
    void set_end_point(double,double);
    void set_start_point(double,double,double);
    void set_end_point(double,double,double);
    Location start_point(){return start_point_;}
    Location end_point(){return end_point_;}
};

#endif // TRACK_H
