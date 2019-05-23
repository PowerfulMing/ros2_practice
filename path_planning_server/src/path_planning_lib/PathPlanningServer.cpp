/*
 * This is how graph looks like:
 *             0  1  2
 *             O--O--O
 *             |  |  |
 *             O--O--O
 *             3  4  5
 */

#include "PathPlanningServer.h"
#include "iostream"

void PathPlanningServer::input_sibling_tree_data()
{
    std::cout << "Inputing sibling tree data... " << std::endl;
    // Vertex 0 sibling
    edge_tmp_ = new edge(1, 0);
    edge_list_tmp_.push_back(*edge_tmp_);
    edge_tmp_ = new edge(3, 1.57);
    edge_list_tmp_.push_back(*edge_tmp_);
    m_sibling_tree_.push_back(edge_list_tmp_);
    edge_list_tmp_.clear();

    // Vertex 1 sibling
    edge_tmp_ = new edge(0, 3.14);
    edge_list_tmp_.push_back(*edge_tmp_);
    edge_tmp_ = new edge(2, 0.0);
    edge_list_tmp_.push_back(*edge_tmp_);
    edge_tmp_ = new edge(4, 1.57);
    edge_list_tmp_.push_back(*edge_tmp_);
    m_sibling_tree_.push_back(edge_list_tmp_);
    edge_list_tmp_.clear();

    // Vertex 2 sibling
    edge_tmp_ = new edge(1, 3.14);
    edge_list_tmp_.push_back(*edge_tmp_);
    edge_tmp_ = new edge(5, 1.57);
    edge_list_tmp_.push_back(*edge_tmp_);
    m_sibling_tree_.push_back(edge_list_tmp_);
    edge_list_tmp_.clear();

    // Vertex 3 sibling
    edge_tmp_ = new edge(0, -1.57);
    edge_list_tmp_.push_back(*edge_tmp_);
    edge_tmp_ = new edge(4, 0.0);
    edge_list_tmp_.push_back(*edge_tmp_);
    m_sibling_tree_.push_back(edge_list_tmp_);
    edge_list_tmp_.clear();

    // Vertex 4 sibling
    edge_tmp_ = new edge(1, -1.57);
    edge_list_tmp_.push_back(*edge_tmp_);
    edge_tmp_ = new edge(3, 3.14);
    edge_list_tmp_.push_back(*edge_tmp_);
    edge_tmp_ = new edge(5, 0);
    edge_list_tmp_.push_back(*edge_tmp_);
    m_sibling_tree_.push_back(edge_list_tmp_);
    edge_list_tmp_.clear();

    // Vertex 5 sibling
    edge_tmp_ = new edge(2, -1.57);
    edge_list_tmp_.push_back(*edge_tmp_);
    edge_tmp_ = new edge(4, 3.14);
    edge_list_tmp_.push_back(*edge_tmp_);
    m_sibling_tree_.push_back(edge_list_tmp_);
    edge_list_tmp_.clear();
}

void PathPlanningServer::input_graph_data()
{
    std::cout << "Inputing Graph data... " << std::endl;
    for (int i=0;i<2;i++) {
        for (int j=0;j<3;j++) {
            unsigned long id = i*3+j;
            struct location location={j,i};
            struct vertex vertex={id, m_sibling_tree_[id], location};

            m_graph.vertex_array_.push_back(vertex);
        }
    }
//    std::cout << m_graph.return_dist(0,1) << std::endl;
}

PathPlanningServer::PathPlanningServer()
{
    path = new std::vector<unsigned long>;
    input_sibling_tree_data();
    input_graph_data();
}

void PathPlanningServer::AStar(unsigned long start_index, unsigned long end_index)
{
    std::cout << "Calculating path... " << std::endl;

    path->clear();
    std::vector<unsigned long> close_list;
    std::priority_queue< std::pair<double, unsigned long>,
                            std::vector<std::pair<double, unsigned long>>,
                            std::greater<std::pair<double, unsigned long>> >open_list;
    double G,H;
    unsigned long current_index = start_index;

    open_list.push(std::make_pair(0,current_index));

    while(!open_list.empty())
    {
        current_index = open_list.top().second;
        open_list.pop();
        close_list.push_back(current_index);

        if(current_index == end_index)
        {

            int tmp_id = current_index;
            path->push_back(tmp_id);
            Path* new_path = new Path();
            new_path->addNewVertex(tmp_id);
            while(tmp_id!=start_index)
            {
                tmp_id = m_graph.vertex_array_[tmp_id].parent;
                path->push_back(tmp_id);
                new_path->addNewVertex(tmp_id);
            }

            std::cout << "The Path is: " << std::endl;
            for(int i=path->size()-1; i>=0;i--)
                std::cout << (*path)[i] << ", ";
            std::cout << std::endl;
            return;
        }

        struct vertex current_vertex = {current_index, m_graph.vertex_array_[current_index].edge_list_, m_graph.vertex_array_[current_index].location_};
        for(unsigned long i=0;i<current_vertex.edge_list_.size();i++)
        {
            unsigned long sibling_id = current_vertex.edge_list_[i].to_node_id;
            std::vector<unsigned long>::iterator it = std::find(close_list.begin(), close_list.end(), sibling_id);
            if(it!=close_list.end())
                continue;
            G = m_graph.return_dist(start_index, sibling_id);
            H = m_graph.return_dist(sibling_id, end_index);
            m_graph.vertex_array_[sibling_id].cost = G+H;
            m_graph.vertex_array_[sibling_id].parent = current_index;
            open_list.push(std::make_pair(m_graph.vertex_array_[sibling_id].cost, sibling_id));
        }
    }
    std::cout << "Nothing find!!!" << std::endl;
}

bool PathPlanningServer::parse_json()
{
    Json::Value root;
    Json::Reader reader;
    bool parsingSuccessful = reader.parse( mas_request_.c_str(), root );     //parse process
    if ( !parsingSuccessful )
    {
        std::cout  << "Failed to parse"
                   << reader.getFormattedErrorMessages();
        return false;
    }
    Json::Value root_info = root["MultiAgentSystemElements"][0];
    if (root_info["typename"].compare("PathPlanningService") != 0)
    {
        // root_info is not "graph" (typename is not "graph")
        return false;
    }
//    Json::Value goalInfoData = root["MultiAgentSystemElements"][0]["data"];

    from_index = root["MultiAgentSystemElements"][0]["data"][0]["from"].asInt();
    to_index = root["MultiAgentSystemElements"][0]["data"][0]["to"].asInt();
    std::cout << "From: " << from_index << std::endl;
    std::cout << "To: " << to_index << std::endl;
    return true;
}

bool PathPlanningServer::parse_request()
{
    if (mas_request_ != "")
    {
        return parse_json();
    }
}

bool PathPlanningServer::generate_json()
{
    Json::Value data_vec(Json::arrayValue);
    Json::Value data;
    for(int i=path->size()-1; i>=0;i--)
    {
        data["typename"] = "vertex";
        data["id"] = static_cast<int>((*path)[i]);
        data["time"] = 0.1*i;
        data_vec.append(data);
    }
    Json::Value MultiAgentSystemElementsVec(Json::arrayValue);
    Json::Value Elements;
    Elements["typename"] = "PathPlanningService";
    Elements["id"] = 0;
    Elements["data"] = data_vec;
    MultiAgentSystemElementsVec.append(Elements);

    Json::Value MultiAgentSystemElements;
    MultiAgentSystemElements["MultiAgentSystemElements"] = MultiAgentSystemElementsVec;
    Json::FastWriter fastWriter;
    mas_response_ = fastWriter.write(MultiAgentSystemElements);
}

std::string PathPlanningServer::handle_request()
{
    AStar(from_index, to_index);
    generate_json();
    //std::cout << "Response this" << mas_response_ << std::endl;
//    mas_response_ = "This is a test!";
    return mas_response_;
}
