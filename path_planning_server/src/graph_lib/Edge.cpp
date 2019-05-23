/**
 * Project Alice
 */


#include "Edge.h"
#include "jsoncpp/json/json.h"

/**
 * Edge implementation
 */
Edge::Edge():RoutingElement ()
{

}

Edge::Edge(const Edge *e):RoutingElement ()
{
    this->edge_index_= e->edge_index_;
    this->vertex_end_index_ = e->vertex_end_index_;
    this->vertex_start_index_ = e->vertex_start_index_;
}

string Edge::to_string()
{
    Json::Value EdgeData;
    Json::Value EdgeValue;
    EdgeValue["StartVertex"] = vertex_start_index_;
    EdgeValue["EndVertex"] = vertex_end_index_;
    EdgeData["typename"] = "Edge";
    EdgeData["id"] = edge_index_;
    EdgeData["value"] = EdgeValue;
    return EdgeData.toStyledString();
}

bool Edge::parse_string( string  input_string )
{
    Json::Value root;
    Json::Reader reader;
    if (!reader.parse(input_string , root))
    {
        return false;
    }

    if (root["typename"].compare("Edge") != 0)
    {
        return false;
    }
    else
    {
        //  if these data type is edge
        Json::Value edgedata = root;
        this->set_edge_index(edgedata["id"].asInt());                          // set edge index
        this->vertex_start_index_ = edgedata["value"]["StartVertex"].asInt();  // set start vertex
        this->vertex_end_index_ = edgedata["value"]["EndVertex"].asInt();      // set end vertex
    }
    
    return true;
}
