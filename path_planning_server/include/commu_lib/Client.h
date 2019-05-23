/**
 * Project Alice
 */

#ifndef _CLIENT_H
#define _CLIENT_H

#include "UniversalClient.h"
#include "Shatter.h"

class Client : public UniversalClient
{
public:
  Client();
  ~Client();
  void send_request(void);
  virtual void create_request(void) = 0;
  void handle_response(void);

protected:
  std::string mas_request_;
  std::string mas_response_;

private:
  //
  // client request/response related
  bool has_request_ = false;
  bool already_send_request_ = false;

  // timer
  BoostTimer<> timer_;
  void main_loop(void);
};

#endif //_CLIENT_H
