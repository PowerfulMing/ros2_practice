/**
 * Project Alice
 */

#ifndef _LOCATION_H
#define _LOCATION_H

class Location
{
  public:
    double x, y, z;
    Location();
    Location(double, double, double);
    void set(Location);
    void set(double, double, double);

    Location operator-(Location l)
    {
        Location ln(this->x - l.x, this->y - l.y, this->z - l.z);
        return ln;
    }
};

#endif //_LOCATION_H