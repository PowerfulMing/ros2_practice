/**
 * Project Alice
 */

#ifndef _SIBLINGTREE_H
#define _SIBLINGTREE_H

#include <boost/container/vector.hpp>
#include <boost/tuple/tuple.hpp>
/*
#include <boost/range/algorithm.hpp>
#include <boost/thread.hpp>
#include <boost/atomic.hpp>
#include <boost/function.hpp>
#include <boost/timer.hpp>
#include <boost/asio.hpp>
*/
#include <iostream>
#include <math.h>
#include <jsoncpp/json/json.h>
#include "RoutingElement.h"
#include "Graph.h"

#define BoostVector boost::container::vector    // vector: for convenience of reading
#define BoostTuple boost::tuples::tuple         // tuple: for convenience of reading
typedef BoostTuple<unsigned long, double> SiblingData; // index 0: is vertex number(unsigned long); index 1: bearing angle(double)
typedef BoostVector<SiblingData> SiblingArray;
typedef BoostVector<SiblingArray> SiblingTable; //

class SiblingTree : public RoutingElement
{
public:
    SiblingTree();
    SiblingTree(Graph *);
    std::string to_string();
    bool parse_string(std::string);
    void setAGVType(std::string);

public:    
    // SiblingTable *sibling_table_;
    SiblingTable sibling_table_;

private:
    Graph graph_;
    std::string AGV_Type_= "MTV";
    //   double distanceBetweenTwoPoint(double x1, double y1, double x2, double y2);
};

#endif //_GRAPH_H
