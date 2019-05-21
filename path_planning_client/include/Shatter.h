/**
 * Project Alice
 */

#ifndef _SHATTER_H
#define _SHATTER_H

#include "Status.h"
#include "Thumper.h"
#include <string>
class Shatter
{
public:
  Shatter()
  {
    local_ = true;
    index_ = 0;
  }
  Shatter(bool l)
  {
    local_ = l;
    index_ = 0;
  }

  std::string name() { return name_; }
  void set_name(std::string sName) { name_ = sName; }
  uint64_t index() { return index_; }
  void set_index(uint64_t iIndex) { index_ = iIndex; }

  Status status() { return status_; }

  virtual void init() = 0;

  virtual void run() = 0;

  virtual void stop() = 0;

protected:
  bool local_;
  std::string name_;
  uint64_t index_;
  Status status_;
  void set_status(Status s) { status_ = s; }
};

#endif //_SHATTER_H