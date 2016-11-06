#include "networklib.h"
#include <iostream>
#include <boost/asio.hpp>
#include <boost/make_shared.hpp>
#include <regex>
//#include <memory>

const std::string browser_name="Suppa_Browser";
const std::string platform="Linux";
const std::string shifr="N";




std::string http(std::string host, std::string page, bool* err)
	{
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

    if (ec==0)
		{
    	//std::cout<<"Get request on the way!";
		std::string req="GET " + page + " HTTP/1.1\r\nHost: "+host+"\r\nAccept: */*\r\nConnection: close\r\n\r\n";
    	//std::string req="GET " + page + " HTTP/1.1\r\n" + "Host: " + host +
          //          	"\r\nUser-Agent: " + ::browser_name + " (" + ::platform + ", " + ::shifr + ", ru)" +
          //          	"\r\nAccept: text/html" + "\r\nContent-Length: 0\r\n" + "Connection: close\r\n\r\n";
		write(sock, buffer(req));
    	boost::asio::streambuf buff;
    	read_until(sock, buff , "\r\n\r\n");
    	//std::vector<std::string> header;
    	std::istream str(&buff);
		std::string now;
		std::getline(str, now);
		//std::cerr<<now<<std::endl;
		std::regex reg("HTTP/1\.1[[:space:]]*([0-9]*)");
		auto str_begin=std::sregex_iterator(now.begin(), now.end(), reg);
		std::sregex_iterator i = str_begin;
		std::smatch match = *i;
		//std::cerr<<match[1]<<std::endl;//"        "<<match[1]<<std::endl;
		std::map<std::string, std::string> header;
		header["status_code"] = match[1];

        std::vector<std::regex> reg_v;
		reg="Date:[[:space:]](.*)";
		reg_v.push_back(reg);
		reg="Server:[[:space:]](.*)";
		reg_v.push_back(reg);
    	reg="Content-Type:[[:space:]](.*)";
		reg_v.push_back(reg);
		reg="Content-Length:[[:space:]]([0-9]*)";
		reg_v.push_back(reg);
		reg="Connection:[[:space:]](.*)";
		reg_v.push_back(reg);
		reg="Location:[[:space:]](.*)";
		reg_v.push_back(reg);
		std::vector<std::string> keywords;
		keywords.push_back("Date");
		keywords.push_back("Server");
		keywords.push_back("Content-Type");
		keywords.push_back("Content-Length");
		keywords.push_back("Connection");
		keywords.push_back("Location");
		std::sregex_iterator end;
		while(str)
		{
			std::getline(str, now);
			for(int k=0; k<reg_v.size(); k++)
			{
				str_begin=std::sregex_iterator(now.begin(), now.end(), reg_v[k]);
				i=str_begin;
				if(i!=end)
				{
					match=*i;
					header[keywords.at(k)]=match[1];
					std::cout<<match[0]<<"      "<<match[1]<<std::endl;
				}			
			}
			
		}
		str.clear();
    	std::string st;
		int size=std::stoi(header.at("Content-Length"));
		boost::shared_ptr<unsigned char[]> body = boost::make_shared<unsigned char[]>(size);
       	boost::system::error_code error;
    	read(sock, buffer(body.get(), size), error);

    	//sock.shutdown(ip::tcp::socket::shutdown_receive);
    	sock.close();
    	std::string res((char*)body.get());
    	return res;
		}

    else 
		{
		*err=true; return ec.message();
		}                                                 //Здесь надо будет возвращать код ошибки
	}

NetworkRes get(std::string site)
	{
    using namespace std;
    //std::cout<<"Maybe it will work";

    string str_http = "http://";
    string str_https = "https://";
    string page = "";
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
    site.erase(pl,site.length()-1);

    string res = http(site, page, &err);
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
