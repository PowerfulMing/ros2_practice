/**
 * Project Alice
 */

#include "Service.h"

/**
 * Service implementation
 */
Service::Service()
{
    set_node_name("Service");
    set_service_name("Service");
    init_timer();
}
Service::~Service()
{
    stop();
}
void Service::init()
{
    UniversalService::init();
}
void Service::run()
{
    UniversalService::start_service();
}
void Service::stop()
{
    UniversalService::shudown();
}