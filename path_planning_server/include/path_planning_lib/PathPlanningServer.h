#ifndef PATH_PLANNING_SERVER_H
#define PATH_PLANNING_SERVER_H

#include "Service.h"
#include "vector"
#include "queue"
#include "algorithm"
#include "path.h"
#include <jsoncpp/json/json.h>
#include <jsoncpp/json/reader.h>
#include <jsoncpp/json/writer.h>
#include <jsoncpp/json/value.h>

//
#include "GraphClient.h"
#include "SiblingTree.h"
#include "SiblingClient.h"

//struct location
//{
//    location(int x, int y):x(x),y(y){}
//    int x;
//    int y;
//};

//struct edge
//{
//    edge(int to_node_id, double angle):to_node_id(to_node_id), angle(angle){}
//    int to_node_id;
//    double angle;
//};

//typedef std::vector<edge> edge_list;
//typedef std::vector<edge_list> sibling_tree_type;

//struct vertex
//{
//    vertex(unsigned long id, edge_list edge_list_, location location_):id_(id), edge_list_(edge_list_),location_(location_){}
//    unsigned long id_;
//    unsigned long parent;
//    edge_list edge_list_;
//    location location_;
//    double G, H;
//    double cost;
//    int occupied;
//};

//class Graph
//{
//public:
//    Graph(){}
//    ~Graph(){}
//    double return_dist(unsigned long Va_id, unsigned long Vb_id)
//    {
//        int x1 = vertex_array_[Va_id].location_.x;
//        int x2 = vertex_array_[Vb_id].location_.x;
//        int y1 = vertex_array_[Va_id].location_.y;
//        int y2 = vertex_array_[Vb_id].location_.y;
//        return sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2));
//    }
//    std::vector<vertex> vertex_array_;

//private:
//};

//typedef std::vector<edge_type> vertex_type;
//typedef std::vector<vertex_type> sibling_tree_type;

class PathPlanningServer : public Service
{
public:
    PathPlanningServer();
    ~PathPlanningServer(){}
    bool parse_request();
    std::string handle_request();

protected:
private:
    Edge *m_edge_;
    Graph m_graph_;
    SiblingTree m_sibling_;
    GraphClient graph_client_;
    SiblingClient sibling_client_;

    // Generate Graph
    void generate_graph(int x, int y);
    void link_vetex(unsigned int id, unsigned int from, unsigned int to);
    double calc_dist(Location a, Location b);

    // For permanent
    int from_index_, to_index_;
    std::vector<unsigned long> *path;

    bool parse_json();
    bool generate_json();
    void AStar(unsigned long start_index, unsigned long end_index);
};

#endif // PATH_PLANNING_SERVER_H
