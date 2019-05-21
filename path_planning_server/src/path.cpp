#include "path.h"
#include <algorithm>
Path::Path()
{
    vertex_index_array_ = new vector<unsigned int>;
    vertex_state_array_ = new vector<VertexState>;
}

Path::Path(const Path * template_path)
{
    int vSize = template_path->vertex_index_array_->size();
    vertex_index_array_ = new vector<unsigned int>;
    vertex_state_array_ = new vector<VertexState>;
    for(int i=0;i<vSize;i++)
    {
        this->vertex_index_array_->push_back(template_path->vertex_index_array_->at(i));
        this->vertex_state_array_->push_back(template_path->vertex_state_array_->at(i));
    }
}

void Path::addNewVertex(unsigned long i)
{
    this->vertex_index_array_->push_back(i);
    this->vertex_state_array_->push_back(VertexState::UNPROCESSED);
}

void Path::mirrorVertexIndexArray()
{
    std::reverse(this->vertex_index_array_->begin(),this->vertex_index_array_->end());
}

void Path::mirrorVertexStateArray()
{
    std::reverse(this->vertex_state_array_->begin(),this->vertex_state_array_->end());
}

