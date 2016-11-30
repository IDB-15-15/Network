#ifndef NETWORKRES_H 
#define NETWORKRES_H

#include <boost/any.hpp>
#include <memory>

struct NetworkRes
{
    boost::any res;         //result - array
    const char *res_arr;    //array's address
    std::size_t size;       //array's size
};

#endif // NETWORKRES_H