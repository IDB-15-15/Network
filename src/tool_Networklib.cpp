#include "MainFoo.h"
#include "networkres.h"
#include <iostream>
#include <memory>
#include <string>
int main()
{
 std::string site="file://C:/Master/Network/src/locallib.cpp";
 NetworkRes res=starter(site);
 std::cout<<res.get_res_arr()<<std::endl;
}
