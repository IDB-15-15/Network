#include "networklib.h"
#include <iostream>
#include <memory>
#include <string>
int main()
{
 std::string site="http://www.ukranews.com/";
 NetworkRes res=get(site);
 std::cout<<res.get_res_arr()<<std::endl;
}
