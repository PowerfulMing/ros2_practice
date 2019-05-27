#include "SiblingTree.h"

SiblingTree::SiblingTree()
{
    // sibling_table_ = new SiblingTable();
}

SiblingTree::SiblingTree(Graph *input_graph)
{
    unsigned int vertex_amount = input_graph->vertex_array_->size();
    sibling_table_.resize(vertex_amount);

    unsigned int edge_size = input_graph->edge_array_->size();

    for (unsigned int i = 0; i < edge_size; i++) // traverse all edge, and fill the table of sibling_tree_
    {
        unsigned int start = input_graph->edge_array_->at(i)->vertex_start_index_;
        unsigned int end = input_graph->edge_array_->at(i)->vertex_end_index_;

        // compute bearing angle between the start node and the end node
        double startLocationX = input_graph->vertex_array_->at(start)->location().x; // location of start v
        double startLocationY = input_graph->vertex_array_->at(start)->location().y;
        double endLocationX = input_graph->vertex_array_->at(end)->location().x;
        double endLocationY = input_graph->vertex_array_->at(end)->location().y;
        double vertexAngleStartToEnd = atan2((endLocationY - startLocationY), (endLocationX - startLocationX));
        double vertexAngleEndToStart = atan2((startLocationY - endLocationY), (startLocationX - endLocationX));

        // fill info. into the sibling_tree_
        SiblingData tupleStartToEnd(end, vertexAngleStartToEnd);
        SiblingData tupleEndToStart(start, vertexAngleEndToStart);
        sibling_table_.at(start).push_back(tupleStartToEnd);
        sibling_table_.at(end).push_back(tupleEndToStart);
    }

    // // for debug
    // std::cout << "Hello sibling tree" << std::endl;
    // // std::cout << sibling_table_ << std::endl;

    // for (unsigned int j = 0; j < sibling_table_.size(); j++)
    // {
    //     std::cout << "Vertex " << j << ": ";
    //     for (unsigned int k = 0; k < sibling_table_.at(j).size(); k++)
    //     {
    //         unsigned int indexTmp;
    //         double angleTmp;
    //         indexTmp = boost::tuples::get<0>(sibling_table_.at(j).at(k));
    //         angleTmp = boost::tuples::get<1>(sibling_table_.at(j).at(k));
    //         std::cout << indexTmp << ", " << angleTmp << "; ";
    //     }
    //     std::cout << std::endl;
    // }

    // std::cout << std::endl;
}

std::string SiblingTree::to_string()
{
    Json::Value root;
    Json::Value siblingInfo; // root["MultiAgentSystemElement"][0]

    for (unsigned int i = 0; i < sibling_table_.size(); i++)
    {        
        Json::Value siblingData;
        for (unsigned int j = 0; j < sibling_table_.at(i).size(); j++)
        {
            siblingData["value"][j]["id"] = Json::Value::UInt64(boost::tuples::get<0>(sibling_table_.at(i).at(j)));
            siblingData["value"][j]["angle"] = boost::tuples::get<1>(sibling_table_.at(i).at(j));
        }
        siblingData["typename"] = AGV_Type_;
        siblingData["id"] = i;

        siblingInfo["data"][i] = siblingData;
    }

    siblingInfo["typename"] = "Sibling";
    siblingInfo["id"] = Json::Value::UInt64(id());
    
    root["MultiAgentSystemElements"][0] = siblingInfo;

    return root.toStyledString();
}


void SiblingTree::setAGVType(std::string agv_type)
{
    AGV_Type_ = agv_type;
}


bool SiblingTree::parse_string(std::string input_string)
{
    Json::Value root;
    Json::Reader reader;
    if (!reader.parse(input_string, root))
    {
        return false;
    }

    Json::Value root_info = root["MultiAgentSystemElements"][0];
    if (root_info["typename"].compare("Sibling") != 0)
    {
        // root_info is not "Sibling" (typename is not "Sibling") 
        std::cout << "Sibling Tree Error: typename is not Sibling "<< std::endl;       
        return false;
    }

    Json::Value siblingInfoData = root["MultiAgentSystemElements"][0]["data"];

    sibling_table_.resize(siblingInfoData.size()); // resize sibling table
    for (unsigned int i = 0; i < siblingInfoData.size(); i++)
    {
        /*
        if (siblingInfoData[i]["typename"].compare("MTV") == 0)
        {
            std::cout << "typename is MTV" << std::endl;
            // tranform json to MTV sibling_tree
        }
        else if (siblingInfoData[i]["typename"].compare("LNV") == 0)
        {
            std::cout << "typename is LNV" << std::endl;
            // tranform json to LNV sibling_tree
        }
        else
        {
            // other typename, error...
            std::cout << "Sibling Parser Error: these AGV type is not included" << std::endl;
            return false;
        }
        */

        unsigned long vertex_index = siblingInfoData[i]["id"].asInt64();
        for (unsigned int j = 0; j < siblingInfoData[i]["value"].size(); j++)
        {
            SiblingData tuple_tmp(siblingInfoData[i]["value"][j]["id"].asInt64(), siblingInfoData[i]["value"][j]["angle"].asDouble());
            sibling_table_.at(vertex_index).push_back(tuple_tmp);
        }
    }
    return true;
}
