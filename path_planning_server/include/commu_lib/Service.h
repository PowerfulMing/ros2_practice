/**
 * Project Alice
 */

#ifndef _SERVICE_H
#define _SERVICE_H

#include "UniversalService.h"
#include "Shatter.h"

class Service : public UniversalService, public Shatter
{
public:
    Service();
    ~Service();

protected:
    bool do_something(std::string str_req,
                      std::string *str_rep)
    {
        mas_request_ = str_req;
        if (parse_request())
        {
            *str_rep = handle_request();
            return true;
        }
        *str_rep = "non-valid request";
        return false;
    }
    // ToBeImplemented
    virtual bool parse_request()
    {
        if (mas_request_ != "")
            return true;
        return false;
    }
    virtual std::string handle_request()
    {
        mas_response_ = mas_request_;
        return mas_response_;
    }

    virtual void init();
    virtual void run();
    virtual void stop();

    std::string mas_request_;
    std::string mas_response_;

private:
};

#endif //_SERVICE_H
