#include "networklib.h"
#include <iostream>
#include <boost/asio.hpp>
#include <boost/make_shared.hpp>
#include <regex>
#include <boost/any.hpp>
#include <string>
#include <stdlib.h>
#include <boost/lexical_cast.hpp>
#include "network.h"

namespace Network{

const std::string browser_name="Suppa_Browser";
const std::string platform="Linux";
const std::string shifr="N";
const std::string error_message_before="<html><head><title>Ошибка!</title></head><body><h1>Страница недоступна. Код ошибки ";
const std::string error_message_after=".</h1></body></html>"; 


NetworkRes http(std::string host, std::string page, bool* err, std::string port) {
    using namespace boost::asio;

    char* body_;
    NetworkRes result;
    io_service service;

    ip::tcp::resolver resolver(service);
    ip::tcp::resolver::query query(host, port);
    ip::tcp::resolver::iterator iter = resolver.resolve( query);
    ip::tcp::endpoint ep = *iter;


    ip::tcp::socket sock(service);
    sock.open(ip::tcp::v4());
    boost::system::error_code ec;
    sock.connect(ep, ec);

    if (ec == 0){
        std::string req = "GET " + page + " HTTP/1.0\r\n" + "Host: " + host +
            "\r\nUser-Agent: " + ::Network::browser_name + " (" + ::Network::platform + ", " + ::Network::shifr +
            ", ru)" + "\r\nAccept: text/html, image/jpeg, image/png" + "\r\nContent-Length: 0\r\n" + "Connection: close\r\n\r\n";

        write(sock, buffer(req));
        boost::asio::streambuf buff;
        read_until(sock, buff , "\r\n\r\n");

        std::istream str(&buff);
        std::string now;
        std::getline(str, now);
        std::regex reg ("HTTP/1\\..[[:space:]]*([0-9]*)");
        auto str_begin = std::sregex_iterator(now.begin(), now.end(), reg);
        std::sregex_iterator i = str_begin;
        std::smatch match = *i;
        std::map<std::string, std::string> header;

        header["status_code"] = match[1];

        reg = "([^:]*):[[:space:]](.*)";
        std::sregex_iterator end;
        while(str) {
            std::getline(str, now);

            if (now == "\r")
                break;

            str_begin = std::sregex_iterator(now.begin(), now.end(), reg);
            i = str_begin;

            if (i == end)
                break;

            match = *i;
            header[match[1]] = match[2];
        }
        str.clear();
        std::string st;
        boost::system::error_code error;

        if ((header["status_code"]=="300")||(header["status_code"]=="301")||(header["status_code"]=="302")||
                (header["status_code"]=="303")||(header["status_code"]=="305")||(header["status_code"]=="307")){
            return Network::give_result(header["Location"]);
            }

        if (header["status_code"] != "200") {
            std::string temp = ::Network::error_message_before + header["status_code"] + error_message_after;
            result.header = header;
            const char* err_mess = temp.c_str();
            result.res_arr = err_mess;
            result.error = true;
        }



        result.header = header;
        if (header.count("Content-Length") != 0) {

            int size = boost::lexical_cast<int>(header.at("Content-Length"));
            boost::shared_ptr<char[]> body = boost::make_shared<char[]> (size);
            size_t ostatok = 0;

            while (str) {
                str.read (body.get() + ostatok, 50);
                ostatok += str.gcount();
            }
            read (sock, buffer(body.get() + ostatok, size - ostatok), error);
            sock.close();
            boost::any res = body;
            body_ = body.get();

            result.size=size;
            result.res = res;
            result.res_arr = body_;

            return result;														
        }
        else {
            size_t readed = 1;
            std::vector<char> vec;
            size_t ostatok = 0;
            while (str) {
                vec.resize (vec.size() + 50);
                str.read (vec.data() + ostatok, 50);
                ostatok += str.gcount();
                vec.resize (vec.size() - 50 + str.gcount());
            }
            boost::system::error_code erread;

            while(readed == 1) {
                vec.resize (vec.size() + 1);
                readed = read (sock, buffer(vec.data() + ostatok, 1), transfer_exactly(1), erread);
                ostatok += 1;
            }
            boost::shared_ptr<std::vector<char>> body=boost::make_shared<std::vector<char>>(vec);
            sock.close();
            boost::any res=body;
            body_=body.get()->data();
            result.res = res;
            result.res_arr = body_;
            result.size=vec.size();

            return result;
        }
    }
    else {
        *err = true;
        boost::any res = ec.message();
        result.res = res;
        result.error = true;

        return result;
    }                                
}

NetworkRes get_network_page(std::string site) {
    using namespace std;
    string page = "";

    bool right = true;
    bool err = false;

    std::regex reg("(.)+:([0-9]{1,4})");
    std::smatch m;
    std::string port="80";
    if (std::regex_match(site, m, reg)){
        port = m.str(2);
        site.erase((site.begin()+m.position(2))-1, site.end());
    }
    for (int i = 8; i < site.length(); i++) {  //Проверяем на наличие адреса страницы
        if (site[i] == '/')
            right = false;
    }

    if (right) {                                //И добавляем, если надо
        site += "/";
    }

    site.erase(0, 7);

    std::string::size_type sl = site.find('/');
    page = site.substr(sl);

    int pl = (int)sl;
    site.erase(pl, site.length() - 1);

    NetworkRes result = http(site, page, &err, port);

    return result;
}

}
