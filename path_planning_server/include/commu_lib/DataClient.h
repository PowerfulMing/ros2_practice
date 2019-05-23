/**
 * Project Alice
 */

#ifndef _DATACLIENT_H
#define _DATACLIENT_H
#include "Client.h"
#include "RequestMode.h"
class DataClient : public Client
{
public:
  DataClient();
  ~DataClient();
  bool data_acquired(void) { return data_acquired_; }
  bool update_success(void) { return update_success_; }
  std::string data_type(void) { return data_type_; }
  void acquire_data(void);
  void update_data(void);

protected:
  void handle_response(void);
  void create_request(void);
  // ToBeImplemented
  virtual void create_data_update_request(void);
  virtual void create_data_acquire_request(void);
  virtual bool parse_response(void);

  void set_data_type(std::string dt) { data_type_ = dt; }
  bool data_acquired_;
  bool update_success_;
  RequestDataMode mode_;
  std::string data_type_;
};

#endif //_DATACLIENT_H
