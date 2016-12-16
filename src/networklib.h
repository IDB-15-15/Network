#ifndef NETWORKLIB_H
#define NETWORKLIB_H

#include <iostream>
#include <memory>
#include <boost/any.hpp>
#include <map>
class NetworkRes{
    int error=0;
    int mode=0;
    boost::any res;
	char *res_arr;
	std::map<std::string, std::string> header;
  public:
    inline NetworkRes & operator=(const NetworkRes &some)=default;
    inline int get_error();
    inline void set_error(int err);
    inline int get_mode();
    inline void set_mode(int mod);
	inline char* get_res_arr();
	inline void push(char *res_);
    inline boost::any get_res();
    inline void push_any(boost::any value);
	inline std::map<std::string, std::string> get_header();
	inline void push_header(std::map<std::string, std::string> heade);

    NetworkRes()=default;
};

#include "networklibfoo.h"



NetworkRes get(std::string site);

#endif // NETWORKLIB_H
