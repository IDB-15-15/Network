#ifndef NETWORKLIB_H
#define NETWORKLIB_H

#include <iostream>
#include <memory>
class NetworkRes{
    int error=0;
    int mode=0;
    std::string res;
  public:
    inline NetworkRes & operator=(const NetworkRes &some)=default;
    inline int get_error();
    inline void set_error(int err);
    inline int get_mode();
    inline void set_mode(int mod);
    inline std::string get_res();
    inline void push(std::string value);
};

#include "networklibfoo.h"



NetworkRes get(std::string site);

#endif // NETWORKLIB_H
