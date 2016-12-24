#include "network.h"
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
    std::regex https ("(https://(.)+)");
    NetworkRes res;

    try{
        std::regex_match(address, mymatches, http);
        if (mymatches.size()) {
            res = get_network_page(address);
            res.site = address;

            return res;
        }

        std::regex_match(address, mymatches, https);
        if (mymatches.size()) {
            res.res_arr="<html><head><title>Ошибка!</title></head><body><h1>Требуемый протокол HTTPS не поддерживается.</h1></body></html>";
            res.size=113;
            res.site=address;
        }


        std::regex_match(address, mymatches, file);
        if (mymatches.size()) {
            std::string addr = address;
            addr.erase(0, 7);
            res = get_local_file(addr);
            res.site = address;

            return res;
        }
        std::regex net ("(www\\.(.)+)|((.)+\\.[a-zA-Z]{1,4}(.)*)");
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
    } catch(const std::exception &e){
           std::cerr<<"ALARM!!!"<<e.what()<<std::endl;
           res.error = 1;
           res.res_arr = "";
           res.site=address;
           res.res_arr="<html><head><title>Ошибка!</title></head><body><h1>Что-то пошло не так. Проверьте интернет-подключение и правильность ввода адреса страницы.</h1></body></html>";
           return res;
    }
}

}
