#include "networklib.h"
#include <iostream>
#include <boost/asio.hpp>
#include <boost/shared_ptr.hpp>
//#include <memory>

const std::string browser_name="Suppa Browser";
const std::string platform="Linux";
const std::string shifr="N";




std::string http(std::string host, std::string page, bool* err){
    using namespace boost::asio;
    io_service service;
    ip::tcp::resolver resolver(service);
    ip::tcp::resolver::query query(host, "80");
    ip::tcp::resolver::iterator iter = resolver.resolve( query);
    ip::tcp::endpoint ep = *iter;
    //ip::tcp::endpoint ep(ip::address::from_string("46.105.108.63"), 80);

    ip::tcp::socket sock(service);
    sock.open(ip::tcp::v4());
    boost::system::error_code ec;
    sock.connect(ep, ec);

    if (ec==0){
    //std::cout<<"Get request on the way!";
    std::string req="GET "+page+" HTTP/1.1\r\n"+"Host: "+host+
                    "\r\nUser-Agent: "+::browser_name+"("+::platform+", "+::shifr+", ru)"+
                    "\r\nAccept: text/html"+"\r\nContent-Length: 0\r\n"+"Connection: close\r\n\r\n";

    write(sock, buffer(req));
    boost::asio::streambuf buff;
    read_until(sock, buff , "\r\n\r\n");
    std::vector<std::string> header;
    std::istream str(&buff);
    std::string http_version;
    str>> http_version;
    //std::cout<<http_version;
    header.push_back(http_version);    //1-ый эл-т хедера - версия HTTP
    unsigned int status_code;
    str >> status_code;
    std::string sti=std::to_string(status_code);
    //std::cout<<sti;
    header.push_back(sti);    //2-ой-статус подключения в виде кода
    std::string status_message;
    std::getline(str, status_message);
    //std::cout<<status_message;
    header.push_back(http_version);     //3-ий-в виде сообщения

    std::string now;
    while (std::getline(str, now) && now != "\r"){
      header.push_back(now);
	  std::cout<<now;
    }
    str.clear();

    int k =3;
    int size;
    std::string st;
    while (k<header.size()){
       	st=header[k];
        st.substr(0, 16);
        if (st=="Content-Length: "){
            std::string::size_type sz; 
			size=std::stoi (header[k], &sz);        
		}
		k++; 
    }
    
	boost::shared_ptr<unsigned char> body=boost::make_shared<unsigned char[]>(size);   //      (new unsigned char[size]);

    boost::system::error_code error;
    read(sock, buffer(body.get(), size), error);
    //sock.shutdown(ip::tcp::socket::shutdown_receive);
    sock.close();
    std::string res((char*)body.get());
    return res;}
    else {*err=true; return ec.message();} //Здесь надо будет возвращать код ошибки
}

NetworkRes get(std::string site){
    using namespace std;
    //std::cout<<"Maybe it will work";

    string str_http="http://";
    string str_https="https://";
    string page="";

    NetworkRes result;                        //Возвращаемый экземпляр класса

    bool right=true;
    bool err=false;
    for (int i=8; i<site.length(); i++)
    {                                         //Проверяем на наличие адреса страницы
            if (site[i]=='/') right=false;
    }
    if (right)
    {                                //И добавляем, если надо
            page="/";
    }

    //if (site.find(str_http)!=site.npos)       //Работа по HTTP
    //{
            //std::cout<<"Get foo enabled!";
            site.erase(0,7);
            std::string::size_type sl = site.find('/');
            page=site.substr(sl);
            int pl = (int)sl;
            site.erase(pl,site.length());
            string res=http(site, page, &err);
            result.set_mode(1);
            result.push(res);

    //}
//    if (site.find(str_https)!=site.npos){       //Работа по HTTP
//            site.erase(0,8);
//            std::string::size_type sl = site.find('/');
//            page=site.substr(sl);
//            int pl = (int)sl;
//            site.erase(pl,site.length());
//            string res=http(site, page, &err);
//            result.set_mode(1);
//            result.push(res);

//        }
    return result;

}
