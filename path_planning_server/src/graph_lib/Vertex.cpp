/**
 * Project Alice
 */

#include "Vertex.h"
#include "jsoncpp/json/json.h"

/**
 * Vertex implementation
 */
Vertex::Vertex():RoutingElement()
{
}

Vertex::Vertex(Location location) : RoutingElement()
{
    this->location_.set(location);
    this->cost_ = -1;
    this->occupied_index_ = -1;
    this->on_path_ = false;
    this->vertex_index_ = -1;
    this->id_ = 0;
}

Vertex::Vertex(const Vertex *v) : RoutingElement()
{
    this->location_.set(v->location_);
    this->cost_ = v->cost_;
    this->occupied_index_ = v->occupied_index_;
    this->on_path_ = v->on_path_;
    this->vertex_index_ = v->vertex_index_;
    this->id_ = v->id_;
}

std::string Vertex::to_string()
{
    Json::Value VertexData;
    Json::Value VertexValue;
    VertexValue["x"] = location().x;
    VertexValue["y"] = location().y;
    VertexValue["y"] = location().z;
    VertexData["typename"] = "Vertex";
    VertexData["id"] = vertex_index_;
    VertexData["value"] = VertexValue;
    return VertexData.toStyledString();
}

bool Vertex::parse_string(std::string input_string)
{
    Json::Value root;
    Json::Reader reader;
    if (!reader.parse(input_string , root))
    {
        return false;
    }

    if (root["typename"].compare("Vertex") != 0 )
    {
        return false;
    }
    else
    {
        // if these data type is vertex
        Json::Value vertexdata = root;
        double vertex_location_x = vertexdata["value"]["x"].asDouble();
        double vertex_location_y = vertexdata["value"]["y"].asDouble();
        double vertex_location_z = vertexdata["value"]["z"].asDouble();
        Location vertex_location = Location(vertex_location_x , vertex_location_y , vertex_location_z);

        this->set_vertex_index(vertexdata["id"].asInt()); // set vertex index
        this->set_location(vertex_location);              // set location
        this->set_cost(0);
    }
    
    return true;
}
