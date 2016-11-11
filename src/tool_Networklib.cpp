#include "networklib.h"
#include <iostream>
#include <memory>
#include <string>
int main()
{
 std::string site="http://www.boost.org/";
 std::map<std::string, std::string> header;
 NetworkRes res=get(site, header);
 std::cout<<res.get_res()<<std::endl;
}
