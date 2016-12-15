#include "networklib.h"
#include <iostream>
#include <boost/asio.hpp>
#include <boost/make_shared.hpp>
#include <regex>
#include <boost/any.hpp>
//#include <memory>

const std::string browser_name="Suppa_Browser";
const std::string platform="Linux";
const std::string shifr="N";




boost::any http(std::string host, std::string page, bool* err, std::map<std::string, std::string>& header, char *body_)
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
		//std::string req="GET " + page + " HTTP/1.1\r\nHost: "+host+"\r\nAccept: *\r\nConnection: close\r\n\r\n";
    	std::string req="GET " + page + " HTTP/1.1\r\n" + "Host: " + host +
                    	"\r\nUser-Agent: " + ::browser_name + " (" + ::platform + ", " + ::shifr + ", ru)" +
                    	"\r\nAccept: text/html" + "\r\nContent-Length: 0\r\n" + "Connection: close\r\n\r\n";
		write(sock, buffer(req));
    	boost::asio::streambuf buff;
    	read_until(sock, buff , "\r\n\r\n");
    	std::istream str(&buff);
		std::string now;
		std::getline(str, now);
		std::regex reg("HTTP/1\.1[[:space:]]*([0-9]*)");
		auto str_begin=std::sregex_iterator(now.begin(), now.end(), reg);
		std::sregex_iterator i = str_begin;
		std::smatch match = *i;
		//std::map<std::string, std::string> header;
		header["status_code"] = match[1];
		std::cerr<<match[1];

        reg="([^:]*):[[:space:]](.*)";
		std::sregex_iterator end;
		while(str)
		{
			std::getline(str, now);
			if (now=="\r") break;
			str_begin=std::sregex_iterator(now.begin(), now.end(), reg);
			i=str_begin;
			if (i==end) break;
			match=*i;
			std::cerr<<now<<std::endl;
			header[match[1]]=match[2];
			std::cerr<<match[1]<<"      "<<match[2]<<std::endl;
		}
std::cerr<<"asdasd"<<std::endl;
		str.clear();
    	std::string st;
		boost::system::error_code error;
		while (str)
		{
			str.read(body.get()+ostatok, 50);
			ostatok+=str.gcount();
		}
		if (header.count("Content-Length")!=0)
		{
			int size=std::stoi(header.at("Content-Length"));
			boost::shared_ptr<char[]> body = boost::make_shared<char[]>(size);
			size_t ostatok=0;
			while (str)
			{
				str.read(body.get()+ostatok, 50);
				ostatok+=str.gcount();
			}
    		read(sock, buffer(body.get()+ostatok, size-ostatok), error);
    		sock.shutdown(ip::tcp::socket::shutdown_receive);
    		sock.close();
    		boost::any res=body;
			body_=body.get();
    		return res;														//исправить на boost::any
		}
		else
		{
			size_t readed=50;
			std::vector<char> vec;
			size_t ostatok=0;
			while (str)
			{
				vec.resize(vec.size()+50);
				str.read(vec.data()+ostatok, 50);
				ostatok+=str.gcount();
				vec.resize(vec.size()-50+str.gcount());
			}
			while(readed==50)
			{
				vec.resize(vec.size()+50);
				readed=read(sock, buffer(vec.data()+ostatok, 50), transfer_exactly(50));
				ostatok+=50;
			}
			boost::shared_ptr<std::vector<char>> body=boost::make_shared<std::vector<char>>(vec);
			sock.shutdown(ip::tcp::socket::shutdown_receive);
    		sock.close();
			boost::any res=body;
			body_=body.get()->data();
			return res;
		}
	}
    else 
		{
		*err=true;
		boost::any res=ec.message();
		return res;
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

	std::map<std::string, std::string> header;
	char *body;
    boost::any res = http(site, page, &err, header, body);
	result.push_header(header);
    result.set_mode(1);
    result.push_any(res);
	result.push(body_);		
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

