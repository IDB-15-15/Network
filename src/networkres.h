#include <boost/any.hpp>
#include <cstddef>

struct NetworkRes
{
    boost::any res;         //ðåçóëüòàò - ìàññèâ
    const char *res_arr;    //àäðåñ ìàññèâà
    std::size_t size;       //ðàçìåð ìàññèâà
};
