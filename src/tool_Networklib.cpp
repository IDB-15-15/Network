#include "MainFoo.h"
#include "networkres.h"
#include <iostream>
#include <memory>
#include <string>
int main()
{
 std::string site="file:///home/dima/some.txt";
 NetworkRes res=starter(site);
 std::cout<<res.get_res_arr()<<std::endl;
}
