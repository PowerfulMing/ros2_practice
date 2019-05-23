/**
 * Project Alice
 */

#ifndef _EDGE_H
#define _EDGE_H
#include "RoutingElement.h"
class Edge : public RoutingElement
{
  public:
    unsigned int edge_index_;
    unsigned int edge_index() { return edge_index_; }
    void set_edge_index(unsigned int i) { edge_index_ = i; }
    unsigned int vertex_start_index_;
    unsigned int vertex_end_index_;
    Edge();
    Edge(const Edge *);
    virtual string to_string();
    virtual bool parse_string(string);
};

#endif //_EDGE_H
