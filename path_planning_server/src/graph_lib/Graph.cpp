/**
 * Project Alice
 */

#include "Graph.h"
#include "jsoncpp/json/json.h"

/**
 * Graph implementation
 */

Graph::Graph() : RoutingElement()
{
    vertex_array_ = new VertexList();
    edge_array_ = new EdgeList();
    track_array_ = new TrackList();
}

std::string Graph::to_string()
{
    unsigned int VertexSize = vertex_array_->size();
    unsigned int EdgeSize = edge_array_->size(); 

    Json::Value root;
    Json::Value GraphInfo;
    GraphInfo["typename"] = "Graph";
    GraphInfo["id"] = Json::UInt64(id());

    // vertex data
    for (unsigned int i = 0; i < VertexSize; i++)
    {
        Json::Value VertexValue;
        VertexValue["x"] = vertex_array_->at(i)->location().x;
        VertexValue["y"] = vertex_array_->at(i)->location().y;
        VertexValue["z"] = vertex_array_->at(i)->location().z;

        Json::Value VertexData;
        VertexData["typename"] = "Vertex";
        VertexData["id"] = i;
        VertexData["value"] = VertexValue;

        GraphInfo["data"][i] = VertexData;
    }

    // edge data
    for(unsigned int i = 0; i < EdgeSize; i++)
    {
        Json::Value EdgeValue;
        EdgeValue["StartVertex"] = edge_array_->at(i)->vertex_start_index_;
        EdgeValue["EndVertex"] = edge_array_->at(i)->vertex_end_index_;

        Json::Value EdgeData;
        EdgeData["typename"] = "Edge";
        EdgeData["id"] = i;
        EdgeData["value"] = EdgeValue;

        GraphInfo["data"][VertexSize+i] = EdgeData;
    }

    root["MultiAgentSystemElements"][0] = GraphInfo;
    return root.toStyledString();
}

bool Graph::parse_string(string input_string)
{
    Json::Value root;
    Json::Reader reader;
    if (!reader.parse(input_string, root))
    {
        return false;
    }

    string object_name = root.begin().name();
    Json::Value root_info = root[object_name][0];
    
    if (root_info["typename"].compare("Graph") != 0)
    {
        // root_info is not "graph" (typename is not "graph")
        return false;
    }

    Json::Value graphInfoData = root_info["data"];

    for (unsigned int i = 0; i < graphInfoData.size(); i++)
    {
        std::string graphInfoData_ToString = graphInfoData[i].toStyledString();
        
        Vertex *V = new Vertex();
        Edge *eg = new Edge();

        if ( V->parse_string(graphInfoData_ToString) ) // if these data type is vertex
        {
            this->vertex_array_->push_back(V); // push to vertex array
        }
        else if ( eg->parse_string(graphInfoData_ToString) ) // if these data type is edge
        {
            eg->set_cost(distanceBetweenTwoPoint(
                this->vertex_array_->at(eg->vertex_start_index_)->location().x,
                this->vertex_array_->at(eg->vertex_start_index_)->location().y,
                this->vertex_array_->at(eg->vertex_end_index_)->location().x,
                this->vertex_array_->at(eg->vertex_end_index_)->location().y));                                                   
            this->edge_array_->push_back(eg); // push to edge array
        }
        else
            return false;
    }

    return true;
}

bool Graph::parse_track_string(string input_string)
{
    Json::Value root;
    Json::Reader reader;
    if (!reader.parse(input_string , root))
    {
        return false;
    }

    string object_name = root.begin().name();

    if ( root[object_name].isMember("Track") )
    {
        // if these data type is Track
        Json::Value trackdata = root[object_name]["Track"];

        for(unsigned int i = 0; i < trackdata.size(); i++)
        {
            Track *T = new Track();
            double start_point_x  = stod(trackdata[i]["StartX"].asString(),0);
            double start_point_y  = stod(trackdata[i]["StartY"].asString(),0);
            double end_point_x    = stod(trackdata[i]["EndX"].asString(),0);
            double end_point_y    = stod(trackdata[i]["EndY"].asString(),0);
            int    track_id       = stoi(trackdata[i]["ID"].asString(),0);
            // set track
            T->set_start_point(start_point_x,start_point_y);
            T->set_end_point(end_point_x,end_point_y);
            T->set_id(track_id);
            this->track_array_->push_back(T);  // push to track array
        }
        GeneratePathGraph();  // generate path graph
        return true;
    }
    else
        return false;
}

void Graph::GeneratePathGraph()
{
    Location startPoint;
    Location endPoint;
    int edgeStartIndex;
    int edgeEndIndex;

    for(unsigned int i = 0; i < track_array_->size() ; i++)
    {
        startPoint = (*track_array_)[i]->start_point();
        endPoint   = (*track_array_)[i]->end_point();
        
        if(!overlapping(startPoint,endPoint))
        {
            Edge *eg = new Edge();
            eg->set_edge_index(this->edge_array_->size());
            eg->set_cost(distanceBetweenTwoPoint(startPoint.x,startPoint.y,endPoint.x,endPoint.y));
            edgeStartIndex = checkVertexExist(startPoint);
            if (edgeStartIndex < 0) 
            {
                Vertex *sV = new Vertex();
                sV->set_vertex_index(this->vertex_array_->size());
                sV->set_location(startPoint);
                sV->set_cost(0);
                this->vertex_array_->push_back(sV);   // push to vertex array
                eg->vertex_start_index_ = sV->vertex_index();
            }
            else
            {
                eg->vertex_start_index_ = edgeStartIndex;
            }

            edgeEndIndex = checkVertexExist(endPoint);
            if (edgeEndIndex < 0) 
            {
                Vertex *eV = new Vertex();
                eV->set_vertex_index(this->vertex_array_->size());
                eV->set_location(endPoint);
                eV->set_cost(0);
                this->vertex_array_->push_back(eV);  // push to vertex array
                eg->vertex_end_index_ = eV->vertex_index();
            }
            else
            {
                eg->vertex_end_index_ = edgeEndIndex;
            }
            this->edge_array_->push_back(eg);    // push to edge array
        }
    }
}

bool Graph::overlapping( Location l1, Location l2)
{
    if (distanceBetweenTwoPoint( l1.x, l1.y, l2.x, l2.y ) < 0.1) 
        return true;
    return false;
}

double Graph::distanceBetweenTwoPoint(double x1, double y1, double x2, double y2)
{
    double difference_x = x2 - x1; // the lengrh of adjacent
    double difference_y = y2 - y1; // the length of opposite

    // by Pythagorean Theorem
    double hypotenuse_square = difference_x * difference_x + difference_y * difference_y;

    return sqrt(hypotenuse_square);
}

int Graph::checkVertexExist(Location l)
{
    Location vPoint;
    unsigned int v_index;
    for( v_index = 0; v_index < this->vertex_array_->size(); v_index++)
    {
        vPoint = (*this->vertex_array_)[v_index]->location();
        if (overlapping(l,vPoint)) 
        {
            return (*this->vertex_array_)[v_index]->vertex_index();
        }
    }
    return -1;
}

bool Graph::parse_track_file(const char* filename)
{
    Json::Reader reader;
    Json::Value root;

    ifstream fin(filename);             // open tracks.json

    if (!fin.is_open())
    {
        return false;
    }

    if (reader.parse(fin,root))              // read tracks.json
    {
        // if it reads successfully , it turns trackdata to string.
        Json::Value trackdata = root;
        this->track_data_string = trackdata.toStyledString();
        fin.close();
        return true;
    }
    else
    {
        fin.close();
        return false;
    }    
}
