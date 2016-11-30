#include <boost/any.hpp>
#include <memory>

struct NetworkRes
{
    boost::any res;         //ðåçóëüòàò - ìàññèâ
    const char *res_arr;    //àäðåñ ìàññèâà
    std::size_t size;       //ðàçìåð ìàññèâà
};
