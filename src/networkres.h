#ifndef NETWORKRES_H 
#define NETWORKRES_H

#include <boost/any.hpp>
#include <memory>
#include <map>

namespace Network{

struct NetworkRes
{
    std::string site;
    boost::any res;         //result - array
    const char *res_arr;    //array's address
    std::size_t size;       //array's size
    int error = 0;
    int mode = 0;
    std::map<std::string, std::string> header;

    inline NetworkRes & operator=(const NetworkRes &some) = default;
    inline int get_error();
    inline void set_error(int err);
    inline int get_mode();
    inline void set_mode(int mod);
    inline const char* get_res_arr();
    inline void push(const char *res_);
    inline boost::any get_res();
    inline void push_any(boost::any value);
    inline std::map<std::string, std::string> get_header();
    inline void push_header(std::map<std::string, std::string> heade);

    NetworkRes() = default;
};

}

#include "networklibfoo.h"
#endif // NETWORKRES_H
