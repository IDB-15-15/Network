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
    bool error = false;
    int mode = 0;
    std::map<std::string, std::string> header;

    inline NetworkRes & operator=(const NetworkRes &some) = default;
    NetworkRes() = default;
};

}


#endif // NETWORKRES_H
