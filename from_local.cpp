#include <boost/any.hpp>
#include <boost/interprocess/file_mapping.hpp>
#include <boost/interprocess/mapped_region.hpp>
#include <iostream>
#include <string>

//using namespace std;

//boost::any local(std::string host, std::string page, bool* err, std::map<std::string, std::string>& header, char *body_)
boost::any local(std::string local_url){
    using namespace boost::interprocess;
    const char *name;
    name = local_url.c_str();

    const char *file_name = name;

    file_mapping m_file(file_name, read_only);
    mapped_region region(m_file, read_only);

    void * addr       = region.get_address();
    std::size_t size  = region.get_size();

    std::cout.write(static_cast<const char *> (addr), size);
    return 0;
}

int main(){
    std::string str;
    std::cout << "n = ";
    std::getline(std::cin, str);
    local(str);
    //"C:/Users/Nekit/Documents/Qt projects/from_local/networklib.h";
    return 0;
}
