/**
 * Project Alice
 */

#include "Location.h"

/**
 * Location implementation
 */
Location::Location()
{
}
Location::Location(double a, double b, double c)
{
    x = a;
    y = b;
    z = c;
}
void Location::set(Location c)
{
    x = c.x;
    y = c.y;
    z = c.z;
}

void Location::set(double a, double b, double c)
{
    x = a;
    y = b;
    z = c;
}
