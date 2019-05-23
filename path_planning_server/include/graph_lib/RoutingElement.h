#ifndef ROUTINGELEMENT_H
#define ROUTINGELEMENT_H
#include "BasicElement.h"

class RoutingElement : public BasicElement
{
public:
    double cost_;
    void set_cost(double c){cost_ = c;}
    double cost(){return cost_;}
    RoutingElement();
};

#endif // ROUTINGELEMENT_H
