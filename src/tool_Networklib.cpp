#include "network.h"
#include "networkres.h"
#include <iostream>
#include <memory>
#include <string>

int main(int argc, char **argv) {
//    std::string site = "file://C:/Master/Network/src/locallib.cpp";
//    Network::NetworkRes res = Network::give_result(site);
//    std::cout << res.get_res_arr() << std::endl;
	try{
		std::string site=argv[1];
		auto res=Network::give_result(site);
		std::cout<<res.res_arr;
	} catch(std::exception &e) {
		std::cerr<<"ALARM!!! "<<e.what()<<std::endl;
		}
}
