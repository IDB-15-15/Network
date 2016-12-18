#include "locallib.h"
#include "networkres.h"
#include <boost/interprocess/file_mapping.hpp>
#include <boost/interprocess/mapped_region.hpp>
#include <boost/interprocess/exceptions.hpp>
#include <string>
#include <utility>
#include <memory>

NetworkRes local(std::string local_url)
{
    using namespace boost::interprocess;
    const char *file_name = local_url.c_str();
    NetworkRes res;
    std::shared_ptr<std::string> error_ptr;

    try{
        file_mapping m_file(file_name, read_only);  //read file
        mapped_region region(m_file, read_only);    //push it to region

        res.res_arr = (static_cast<const char *> (region.get_address()));   //save array's address
        res.size = region.get_size();                                       //save array's size
        res.res = std::make_shared<std::pair<file_mapping, mapped_region>>
                (std::move (m_file), std::move (region));                   //save array
    }
    catch(interprocess_exception &e){
        error_ptr = std::make_shared<std::string>                           //create error's message
                ("<html>" + static_cast<std::string> (e.what()) + "</html>");

        res.res = error_ptr;                //save error's message
        res.size = error_ptr->size();       //save error's message size
        res.res_arr = error_ptr->c_str();   //save error's message address
    }

    return res;
}