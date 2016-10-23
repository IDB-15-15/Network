#include "networklib.h"
#include <iostream>
#include <boost/asio.hpp>

const std::string browser_name="Suppa Browser";
const std::string platform="Linux";
const std::string shifr="N";


class Network_res{
    int error=0;
  public:
    int get_error(){
        return error;
    }
    void set_error(int err){
        error=err;
    }
};

std::string http(std::string host, std::string page, bool* err){
    using namespace boost::asio;
    io_service service;
    ip::tcp::resolver resolver(service);
    ip::tcp::resolver::query query(host, "80");
    ip::tcp::resolver::iterator iter = resolver.resolve( query);
    ip::tcp::endpoint ep = *iter;

    ip::tcp::socket sock(service);
    //sock.open(ip::tcp::v4());
    boost::system::error_code ec;
    sock.connect(ep, ec);

    if (ec==0){
    std::string req="Get "+page+" HTTP/1.1\r\n"+"Host: "+host+
                    "User-Agent: "+::browser_name+"("+::platform+", "+::shifr+", ru)"+
                    "Accept: text/html"+"Content-Length: 0";

    sock.write_some(buffer(req));
    char buff[1024];
    sock.read_some(buffer(buff,1024));
    std::string res=buff;
    sock.shutdown(ip::tcp::socket::shutdown_receive);
    sock.close();
    return res;}
    else {*err=true; return ec.message();} //Здесь надо будет возвращать код ошибки
}

Network_res get(std::string site){
    using namespace std;

    string str_http="HTTP://";
    string str_https="HTTPS://";
    string page="";

    Network_res result;                        //Возвращаемый экземпляр класса

    bool right=true;
    bool err=false;
    for (int i=8; i<site.length(); i++){       //Проверяем на наличие адреса страницы
            if (site[i]=='/') right=false;
        }
    if (right){                                //И добавляем, если надо
            page="/";
    }

    if (site.find(str_http)!=site.npos){       //Работа по HTTP
            site.erase(0,7);
            std::string::size_type sl = site.find('/');
            page=site.substr(sl);
            int pl = (int)sl;
            site.erase(pl,site.length());
            string res=http(site, page, &err);
        }



}
