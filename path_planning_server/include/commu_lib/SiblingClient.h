/**
 * Project Alice
 */

#ifndef _SIBLINGCLIENT_H
#define _SIBLINGCLIENT_H

#include "DataClient.h"
#include "SiblingTree.h"
#include "jsoncpp/json/json.h"
#include <iostream>
using namespace std;

class SiblingClient : public DataClient
{
public:
  SiblingClient(void);

  enum RequestType
  {
    kUpdating,
    kGetting,
  };

  void create_data_update_request(void);  // transform Graph to Json format and send to GraphServer to "updating map"
  void create_data_acquire_request(void); // send a request to GraphServer for "getting map"
  bool parse_response(void);              // parse the response from server

  bool set_sibling(SiblingTree *s);
  bool set_sibling(std::string ss);

protected:
  SiblingTree sibling_tree_;
};

#endif //_SIBLINGCLIENT_H