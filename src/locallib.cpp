#include "locallib.h"
#include "networkres.h"
#include <boost/interprocess/file_mapping.hpp>
#include <boost/interprocess/mapped_region.hpp>
#include <boost/interprocess/exceptions.hpp>
#include <boost/asio/error.hpp>
#include <boost/filesystem.hpp>
#include <boost/system/error_code.hpp>
#include <string>
#include <utility>
#include <memory>

namespace Network{
/*
bool path(const char* p)
{
    boost::system::error_code code1 = make_error_code(
                boost::system::errc::no_such_file_or_directory);
    boost::system::error_code code2 = make_error_code(
                boost::asio::error::host_not_found_try_again);

    // которые имеют две различные категории
    assert(code1.category() != code2.category());
    assert(code1.default_error_condition().category() !=
            code2.default_error_condition().category());

    // которые имеют одинаковое значение
    assert(code1.value() == code2.value());
    assert(code1.default_error_condition().value() ==
           code2.default_error_condition().value());

    // Используется операция сравнения для проверки значения и категории
    assert(code1 != code2);
    assert(code1.default_error_condition() !=
            code2.default_error_condition());

    // сама проверка
    try                                    // в каноникал поступает файл
    {
        std::string file = p;
        boost::filesystem::canonical(file);
    }
    catch(boost::filesystem::filesystem_error& error)        // выдает 0 если ошибка
    {
        if (error.code() ==
                make_error_code(boost::system::errc::no_such_file_or_directory))
        {
            return false;
        }
        if (error.code() ==
                make_error_code(boost::asio::error::host_not_found_try_again))
        {
            return  false;
        }
    }
    return true;
}*/

NetworkRes get_local_file(std::string local_url)
{
    using namespace boost::interprocess;
    const char *file_name = local_url.c_str();
    NetworkRes res;
    std::shared_ptr<std::string> error_ptr;	
    /*bool check;
    check = path(file_name);
	
    if(!check){
		error_ptr = std::make_shared<std::string>              //create error's message
               ("<html><title>Error</title><body>ERROR! PATH IS NOT CANONICAL</body></html>");

		res.res = error_ptr;                //save error's message
		res.size = error_ptr->size();       //save error's message size
		res.res_arr = error_ptr->c_str();
	}
    else{ */
		try{
			file_mapping m_file (file_name, read_only);  //read file
			mapped_region region (m_file, read_only);    //push it to region

			res.res_arr = (static_cast<const char *> (region.get_address()));   //save array's address
			res.size = region.get_size();                                       //save array's size
			res.res = std::make_shared<std::pair<file_mapping, mapped_region>>
					(std::move (m_file), std::move (region));                   //save array
		}
		catch(interprocess_exception &e){
			error_ptr = std::make_shared<std::string>                           //create error's message
					("<html><title>Error</title><body>" + static_cast<std::string> (e.what()) + "</body></html>");

			res.res = error_ptr;                //save error's message
			res.size = error_ptr->size();       //save error's message size
			res.res_arr = error_ptr->c_str();   //save error's message address
		}
	//}

    return res;
}

}

