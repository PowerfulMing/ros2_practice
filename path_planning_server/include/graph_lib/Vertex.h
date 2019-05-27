/**
 * Project Alice
 */

#ifndef _VERTEX_H
#define _VERTEX_H
#include "RoutingElement.h"
#include "Location.h"

class Vertex : public RoutingElement
{
  public:
    int vertex_index_;
    int vertex_index() { return vertex_index_; }
    void set_vertex_index(int i) { vertex_index_ = i; }
    Location location_;
    void set_location(Location l) { location_.set(l); }
    Location location() { return location_; }
    int occupied_index_ = -1;
    bool on_path_ = false;
    unsigned long parent;
    double cost;
    Vertex();
    Vertex(Location);
    Vertex(const Vertex *);
    virtual std::string to_string();
    virtual bool parse_string(std::string);
};

#endif //_VERTEX_H
