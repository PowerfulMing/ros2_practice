/**
 * Project Alice
 */

#ifndef _GRAPHCLIENT_H
#define _GRAPHCLIENT_H

#include "DataClient.h"
#include "Graph.h"
#include "jsoncpp/json/json.h"
#include <iostream>
using namespace std;

class GraphClient : public DataClient
{
public:
  GraphClient(void);

  enum RequestType
  {
    kUpdatingMap,
    kGettingMap,
  };

  void create_data_update_request(void);  // transform Graph to Json format and send to GraphServer to "updating map"
  void create_data_acquire_request(void); // send a request to GraphServer for "getting map"
  bool parse_response(void);              // parse the response from server

  bool set_graph(std::string gs);
  bool set_graph(Graph *g);
  void print_data()
  {
    std::cout << graph_.to_string() << std::endl;
  }

protected:
  Graph graph_;
};

#endif //_GRAPHCLIENT_H
