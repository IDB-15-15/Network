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




NetworkRes http(std::string host, std::string page, bool* err)
	{
    using namespace boost::asio;
	char* body_;
	NetworkRes result;
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
		std::map<std::string, std::string> header;
		header["status_code"] = match[1];
		std::cerr<<match[1]<<std::endl;

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
			//std::cerr<<match[1]<<"      "<<match[2]<<std::endl;
		};
		str.clear();
    	std::string st;
		boost::system::error_code error;

	//std::cerr<<"Header is here!"<<std::endl;

		result.push_header(header);

		//while (str)
		//{
			//str.read(body.get()+ostatok, 50);
			//ostatok+=str.gcount();
		//}
		if (header.count("Content-Length")!=0)
		{
			int size=std::stoi(header.at("Content-Length"));
			boost::shared_ptr<char[]> body = boost::make_shared<char[]>(size);
			size_t ostatok=0;
	//std::cerr<<"Content-length est"<<std::endl;
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
			result.push_any(res);
			result.push(body_);
    		return result;														//исправить на boost::any
		}
		else
		{
			size_t readed=1;
			std::vector<char> vec;
			size_t ostatok=0;
	//std::cerr<<"Net ego"<<std::endl;
			while (str)
			{
				vec.resize(vec.size()+50);
				str.read(vec.data()+ostatok, 50);
				ostatok+=str.gcount();
				vec.resize(vec.size()-50+str.gcount());
			}
	//std::cerr<<"Ya chital"<<std::endl;
			boost::system::error_code erread;
			while(readed==1)
			{
				vec.resize(vec.size()+1);
				readed=read(sock, buffer(vec.data()+ostatok, 1), transfer_exactly(1), erread);
				ostatok+=1;
			}
	//std::cerr<<"I vnov prodoljaetsya boy!"<<std::endl;
			boost::shared_ptr<std::vector<char>> body=boost::make_shared<std::vector<char>>(vec);
			sock.shutdown(ip::tcp::socket::shutdown_receive);
    		sock.close();
			boost::any res=body;
			body_=body.get()->data();
			result.push_any(res);
			result.push(body_);
			return result;
		}
	}
    else 
		{
		*err=true;
		boost::any res=ec.message();
		result.push_any(res);
		result.set_error(true); 
		return result;
		}                                                 //Здесь надо будет возвращать код ошибки
	}

NetworkRes get(std::string site)
	{
    using namespace std;
    //std::cout<<"Maybe it will work";

    string str_http = "http://";
    string str_https = "https://";
    string page = "";

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
    NetworkRes result=http(site, page, &err);

	
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

