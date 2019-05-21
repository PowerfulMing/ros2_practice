#ifndef PATH_H
#define PATH_H
#include <vector>
using namespace std;
class Path
{
public:
    enum VertexState{
        UNPROCESSED,
        PROCESSING,
        DONE,
        UNKNOWN
    };
    Path();
    Path(const Path*);
    vector<unsigned int> *vertex_index_array_;
    vector<VertexState> *vertex_state_array_;
    double total_cost_;
    void set_total_cost(double d){total_cost_ = d;}
    double total_cost(){return total_cost_;}
    void addNewVertex(unsigned long);
    void mirrorVertexIndexArray();
    void mirrorVertexStateArray();
    bool path_availible(){
        if(vertex_index_array_ == nullptr ||
           vertex_state_array_ == nullptr )
        {
            return false;
        }
        if(vertex_index_array_->size()<1  ||
           vertex_state_array_->size()<1)
        {
            return false;
        }
        return true;
    }
    VertexState getFirstVertexState(){
        return vertex_state_array_->front();
    }
    void removeFirst(){

        removeFirstVertex();
        removeFirstVertexState();
    }
    void removeFirstVertex(){
        if(!vertex_index_array_->empty())
            vertex_index_array_->erase(vertex_index_array_->begin());
    }
    void removeFirstVertexState(){
        if(!vertex_state_array_->empty())
            vertex_state_array_->erase(vertex_state_array_->begin());
    }
    void processFirstVertex(){
        if(!vertex_state_array_->empty())
            vertex_state_array_->front() = PROCESSING;
    }
    void finishFirstVertex(){
        if(!vertex_state_array_->empty())
            vertex_state_array_->front() = DONE;
    }

};

#endif // PATH_H
