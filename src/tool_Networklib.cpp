#include "networklib.h"
#include <iostream>
#include <memory>
#include <string>
int main()
{
 std::string site="http://ukranews.com/";
 NetworkRes res=get(site);
 //std::cout<<res.get_res();
}
