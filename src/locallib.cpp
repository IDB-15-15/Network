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
#include <sstream>
#include <ctime>

namespace Network{

bool path (std::string p) {
  // сама проверка
    try {                                   // в каноникал поступает файл
        boost::filesystem::canonical(p);
    }
    catch(boost::filesystem::filesystem_error& error) {       // выдает 0 если ошибка
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
}

std::string directory (std::string path) {
    time_t time;
    std::string str;
    std::string path2 = "<html><head></head><body><h1> Содержание ";
    path2 += path;
    path2 += "</h1><br><br>";
    path2 += "<a href=";
    path2 += path;
    path2 += "..>Родительский каталог</a><br>";
    for (auto & p : boost::filesystem::directory_iterator(path)) {
        if (is_regular_file(p)) {
            std::stringstream ss;
            const boost::filesystem::directory_entry& entry = p;
            str = entry.path().string();
            path2 += str;
            path2 += "   ";
            ss << file_size(p);
            path2 += ss.str();
            path2 += " byte    ";
            time = last_write_time(p);
            str = ctime(&time);
            path2 += str;
            path2 += "<br>";
            // cout << p << "  " << file_size(p) <<" byte "<<ctime(&time)<<"\n";
        }
        else if (is_directory(p)) {
            const boost::filesystem::directory_entry& entry = p;
            path2 += "<a href=file://";
            str = entry.path().string();
            path2 += str;
            path2 += ">";
            path2 += str;
            path2 += "   is directory   ";
            time = last_write_time(p);
            str = ctime(&time);
            path2 += str;
            path2 += "</a>";
            path2 += "<br>";

            //cout << p << " is a directory " <<ctime(&time)<<"\n";
        }
    }
    path2 += "</body></html>";

    return path2;
}

NetworkRes get_local_file(std::string local_url)
{
    using namespace boost::interprocess;
    std::string path2, error, path1 = local_url;
    boost::filesystem::path p(path1);
    NetworkRes res;
    std::shared_ptr<std::string> error_ptr;
    bool check;
    check = path(path1);

    if (!check) {
        error_ptr = std::make_shared<std::string>                           //create error's message
            "<html><title>Error</title><body>ERORR! WRONG PATH!!!</body></html>");
        res.res = error_ptr;                //save error's message
        res.size = error_ptr->size();       //save error's message size
        res.res_arr = error_ptr->c_str();
		res.error = true;
        return res;
    }
    else if (is_directory (p)) {
        path2 = directory(path1);
        res.res = path2;
        res.size = path2.size();
        res.res_arr = path2.c_str();
	    res.error = true;
    }
    else {
        const char *file_name = local_url.c_str();
        try {
            file_mapping m_file (file_name, read_only);  //read file
            mapped_region region (m_file, read_only);    //push it to region

            res.res_arr = (static_cast<const char *> (region.get_address()));   //save array's address
            res.size = region.get_size();                                       //save array's size
            res.res = std::make_shared<std::pair<file_mapping, mapped_region>>
                    (std::move (m_file), std::move (region));                   //save array
        }
        catch (interprocess_exception &e) {
            error_ptr = std::make_shared<std::string>                           //create error's message
                ("<html><title>Error</title><body>" + static_cast<std::string> (e.what()) + "</body></html>");

            res.res = error_ptr;                //save error's message
            res.size = error_ptr->size();       //save error's message size
            res.res_arr = error_ptr->c_str();   //save error's message address
			res.error = true;
        }
    }
    return res;
}

}

