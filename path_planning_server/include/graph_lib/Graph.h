/**
 * Project Alice
 */

#ifndef _GRAPH_H
#define _GRAPH_H

#include "Vertex.h"
#include "Edge.h"
#include <boost/container/vector.hpp>
#include <boost/range/algorithm.hpp>
#include <boost/thread.hpp>
#include <boost/atomic.hpp>
#include <boost/function.hpp>
#include <boost/timer.hpp>
#include <boost/asio.hpp>
#include <iostream>
#include <fstream>
#include <string> 
#include <math.h>
#include "RoutingElement.h"
#include "Track.h"

typedef boost::container::vector<Vertex *> VertexList;
typedef boost::container::vector<Edge *> EdgeList;
typedef boost::container::vector<Track *> TrackList;
typedef boost::container::vector<boost::container::vector<unsigned long>> SiblingIndexTree;
class Graph : public RoutingElement
{
public:
  Graph();
  string track_data_string;
  VertexList *vertex_array_; // for debug, it should put on private
  EdgeList *edge_array_; // for debug, it should put on private
  TrackList *track_array_;
  int checkVertexExist(Location);
  bool parse_track_string(string);
  bool parse_track_file(const char*);
  bool overlapping(Location,Location);
  void add_vertex(Vertex *v)
  {
    v->set_vertex_index(vertex_array_->size());
    vertex_array_->push_back(v);
  }
  void add_edge(Edge *e)
  {
    e->set_edge_index(edge_array_->size());
    edge_array_->push_back(e);
  }
  void GeneratePathGraph();
  double distanceBetweenTwoPoint(double, double, double, double);
  virtual string to_string();
  virtual bool parse_string(string);

private:  
  // VertexList *vertex_array_; // To Vincent: here is the original place
  // EdgeList *edge_array_; // To Vincent: here is the soriginal place
  SiblingIndexTree sibling_index_array_;
  SiblingIndexTree sibling_edge_index_array_;
};

#endif //_GRAPH_H
