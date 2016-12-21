#include "MainFoo.h"
#include <iostream>
#include <regex>
#include "locallib.h"
#include "networklib.h"
#include "networkres.h"

namespace Network{
NetworkRes starter(std::string address)
{
	std::smatch mymatches;
  	std::regex http ("(http://(.)+)");
	std::regex file ("(file://(.)+)");
	NetworkRes res;	

	std::regex_match(address, mymatches, http);
	if (mymatches.size())
	{
		res=get(address);
		res.site=address;
		return res;
	}

	std::regex_match(address, mymatches, file);
	if (mymatches.size())
	{
		std::string adr=address;
		adr.erase(0, 7);
std::cerr<<adr;
		res=local(adr);
		res.site=address;
		return res;
	}
	std::regex net ("(www\\.(.)+)|((.)?\\.[a-zA-Z]{1,4}(.)*)");
	//std::regex net ("(www\.(.)+)|((.)?\.[a-zA-Z]{1,4}(.)*)");
	std::regex_match(address, mymatches, net);
	if(mymatches.size())
	{
		std::string adr="http://"+address;
		res=get(adr);
		res.site=adr;
		return res;	
	}
	else
	{
		std::string adr="file://"+address;
		res=local(address);
		res.site=adr;
		return res;
	}	
}
}
