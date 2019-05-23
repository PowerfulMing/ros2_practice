#ifndef BASICELEMENT_H
#define BASICELEMENT_H
#include <iostream>
#include <string>
using namespace std;
class BasicElement
{
  public:
    unsigned long id_;
    void set_id(unsigned long i) { id_ = i; }
    unsigned long id() { return id_; }
    BasicElement();
    BasicElement(int);
    ~BasicElement() { ; }
    virtual std::string to_string() = 0;
    virtual bool parse_string(std::string) = 0;
};

#endif // BASICELEMENT_H
