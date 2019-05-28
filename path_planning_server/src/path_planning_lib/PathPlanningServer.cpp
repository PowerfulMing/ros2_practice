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

PathPlanningServer::PathPlanningServer()
{
    path = new std::vector<unsigned long>;
    m_edge_ = new Edge;
    // Create 6 * 6 graph
    generate_graph(6, 6);
//    for(int i=0;i<m_sibling_.sibling_table_.size();i++)
//        std::cout << "vertex " << i << " has " << m_sibling_.sibling_table_.at(i).size() << " siblings" << std::endl;
}

void PathPlanningServer::generate_graph(int x, int y)
{
    int edge_id=0;
    int current_index = 0;
    for(unsigned int i=0;i < y; i++)
    {
        for(unsigned int j=0; j< x; j++)
        {
            current_index = i*6+j;
            m_graph_.add_vertex(new Vertex(Location(100*j, 100*i, 2)),current_index);
        }
    }

    for(unsigned int m=0; m<x*y; m++)
    {
        for(unsigned int n=m; n<x*y; n++)
        {
            if(m==n)
            {
                // Don't Compare with itself
               continue;
            }
            double dist = calc_dist(m_graph_.vertex_array_->at(m)->location(),m_graph_.vertex_array_->at(n)->location());
            if(dist <= 142)
            {
                if(!m_graph_.if_connected(m,n))
                {
                    link_vetex(edge_id, m, n);
                    edge_id++;
                }else{
//                    std::cout << "Vertex " << m << " and Vertex " << n << " already connected!" << std::endl;
                }
            }else{
//                std::cout << "Vertex " << m << " and Vertex " << n << " are too far!" << std::endl;
            }
        }
    }
//    std::cout << "How many Edge: " << edge_id << std::endl;
//    for(int i=0; i<edge_id; i++)
//    {
//        std::cout << "Link from vertex " << m_graph_.edge_array_->at(i)->vertex_start_index_
//                  << " to vertex " << m_graph_.edge_array_->at(i)->vertex_end_index_ << std::endl;
//    }

    m_sibling_ = SiblingTree(&m_graph_);
    m_sibling_.setAGVType("AMR");
}

double PathPlanningServer::calc_dist(Location a, Location b)
{
    double d_x = a.x-b.x;
    double d_y = a.y-b.y;
    return hypot(d_x,d_y);
}

void PathPlanningServer::link_vetex(unsigned int id, unsigned int from, unsigned int to)
{
    m_edge_ = new Edge();
    m_edge_->set_edge_index(id);
    m_edge_->vertex_start_index_=from;
    m_edge_->vertex_end_index_ = to;
    m_graph_.add_edge(m_edge_);
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

        // Reached Goal! Lookback Parent and Find the Path
        if(current_index == end_index)
        {
            int tmp_id = current_index;
            path->push_back(tmp_id);
            Path* new_path = new Path();
            new_path->addNewVertex(tmp_id);
            while(tmp_id!=start_index)
            {
                m_graph_.vertex_array_;
                tmp_id = m_graph_.vertex_array_->at(tmp_id)->parent;
                path->push_back(tmp_id);
                new_path->addNewVertex(tmp_id);
            }

            std::cout << "The Path is: " << std::endl;
            for(int i=path->size()-1; i>=0;i--)
                std::cout << (*path)[i] << ", ";
            std::cout << std::endl;
            return;
        }

        // Lookup the current vertex's sibling and calculate the cost of each sibling
        for(unsigned int i=0; i<m_sibling_.sibling_table_.at(current_index).size(); i++)
        {
            unsigned long sibling_id = boost::tuples::get<0>(m_sibling_.sibling_table_.at(current_index).at(i));
            // If the vertex already in Close List, skip it
            std::vector<unsigned long>::iterator it = std::find(close_list.begin(), close_list.end(), sibling_id);
            if(it!=close_list.end())
                continue;

            G = m_graph_.return_dist(start_index, sibling_id);
            H = m_graph_.return_dist(sibling_id, end_index);
            m_graph_.vertex_array_->at(sibling_id)->cost = G+H;
            m_graph_.vertex_array_->at(sibling_id)->parent = current_index;
            open_list.push(std::make_pair(m_graph_.vertex_array_->at(sibling_id)->cost, sibling_id));
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

    from_index_ = root["MultiAgentSystemElements"][0]["data"][0]["from"].asInt();
    to_index_ = root["MultiAgentSystemElements"][0]["data"][0]["to"].asInt();
    std::cout << "From: " << from_index_ << std::endl;
    std::cout << "To: " << to_index_ << std::endl;
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
    AStar(from_index_, to_index_);
    generate_json();
    //std::cout << "Response this" << mas_response_ << std::endl;
//    mas_response_ = "This is a test!";
    return mas_response_;
}
