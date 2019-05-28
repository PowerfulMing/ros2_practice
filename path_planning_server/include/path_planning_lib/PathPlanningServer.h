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

class PathPlanningServer : public Service
{
public:
    PathPlanningServer();
    ~PathPlanningServer(){}
    void init() { Service::init(); }
    void run() { Service::run(); }
    void stop() { Service::stop(); }
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
