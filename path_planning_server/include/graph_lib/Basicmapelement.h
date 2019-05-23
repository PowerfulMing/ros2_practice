/**
 * Project Alice
 */

#ifndef BASICMAPELEMENT_H
#define BASICMAPELEMENT_H

class BasicMapElement
{
public:
    unsigned long id_;
    void set_id(unsigned long i){id_ = i;}
    unsigned long id(){return id_;}
    BasicMapElement();
    BasicMapElement(int);
    ~BasicMapElement() {;}
};

#endif // BASICMAPELEMENT_H
