#include "locallib.h"
#include <iostream>

int main()
{
    std::string address;
    std::cout << "n = ";
    std::getline(std::cin, address);

    Network::NetworkRes result = Network::get_local_file(address);
    std::cout.write(result.res_arr, result.size);

    //"C:/Users/Nekit/Documents/Qt projects/untitled/networklib.h";
    return 0;
}
