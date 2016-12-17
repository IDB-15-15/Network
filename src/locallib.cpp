#include "networkres.h"
#include <boost/interprocess/file_mapping.hpp>
#include <boost/interprocess/mapped_region.hpp>
#include <string>
#include <utility>
#include <memory>

NetworkRes local(std::string local_url)
{
    using namespace boost::interprocess;
    const char *file_name = local_url.c_str();	//get file name

    file_mapping m_file(file_name, read_only);  //read file
    mapped_region region(m_file, read_only);    //push it to region

    NetworkRes res;                             //create object of save result class

    res.res_arr = (static_cast<const char *> (region.get_address()));   //save array's address
    res.size = region.get_size();                                       //save size
    res.res = std::make_shared<std::pair<file_mapping, mapped_region>>
            (std::move (m_file), std::move (region));                   //save array

    return res;
}
