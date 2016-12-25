#ifndef directory_h
#define directory_h
#include <string>
#include <iostream>
#include <sstream>
#include <ctime>
#include <boost/filesystem.hpp>
namespace fs = boost::filesystem;
namespace Mydir{
std::string directory(std::string path){
time_t time;
    std::string str;
    std::string path2="<html><head></head><body><h1> Содержание ";
    path2+=path;
    path2+="</h1><br><br>";
    for (auto & p : fs::directory_iterator(path)){
        if (is_regular_file(p))     {
        std::stringstream ss;
            const fs::directory_entry& entry = p;
            str= entry.path().string();
            path2+=str;
            path2+="   ";
            ss<<file_size(p);
            path2+=ss.str();
            path2+=" byte    ";
            time=last_write_time(p);
            str=ctime(&time);
            path2+=str;
            path2+="<br>";
           // cout << p << "  " << file_size(p) <<" byte "<<ctime(&time)<<"\n";
        }
        else if (is_directory(p))      {
            const fs::directory_entry& entry = p;
            path2+="<a href=file://";
            str= entry.path().string();
            path2+=str;
            path2+=">";
            path2+=str;
            path2+="</a>";
            path2+="   is directory   ";
            time=last_write_time(p);
            str=ctime(&time);
            path2+=str;
            path2+="<br>";
            //cout << p << " is a directory " <<ctime(&time)<<"\n";
        }
    }
    path2+="</body></html>";
    return path2;
    }

}
#endif // directory_h
