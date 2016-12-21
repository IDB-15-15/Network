#include "NETWORK.h"
#include <iostream>
#include <regex>
#include "locallib.h"
#include "networklib.h"
#include "networkres.h"

namespace Network{

NetworkRes give_result(std::string address) {
    std::smatch mymatches;
    std::regex http ("(http://(.)+)");
    std::regex file ("(file://(.)+)");
    NetworkRes res;

    std::regex_match(address, mymatches, http);
    if (mymatches.size()) {
        res = get_network_page(address);
        res.site = address;

        return res;
    }

    std::regex_match(address, mymatches, file);
    if (mymatches.size()) {
        std::string addr = address;
        addr.erase(0, 7);
        std::cerr << addr;
        res = get_local_file(addr);
        res.site = address;

        return res;
    }
    std::regex net ("(www\\.(.)+)|((.)?\\.[a-zA-Z]{1,4}(.)*)");
    std::regex_match(address, mymatches, net);

    if (mymatches.size()) {
        std::string addr = "http://" + address;
        res = get_network_page(addr);
        res.site = addr;

        return res;
    }
    else {
        std::string addr="file://" + address;
        res = get_local_file(address);
        res.site = addr;

        return res;
    }
}

}
