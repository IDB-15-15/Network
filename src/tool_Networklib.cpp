#include "NETWORK.h"
#include "networkres.h"
#include <iostream>
#include <memory>
#include <string>

int main() {
    std::string site = "file://C:/Master/Network/src/locallib.cpp";
    Network::NetworkRes res = Network::give_result(site);
    std::cout << res.get_res_arr() << std::endl;
}
