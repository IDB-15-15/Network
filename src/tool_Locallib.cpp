#include "locallib.h"
#include <iostream>

int main()
{
    std::string address;
    std::cout << "n = ";
    std::getline(std::cin, address);

    NetworkRes result = local(address);
    std::cout.write(result.res_arr, result.size);

    //"C:/Users/Nekit/Documents/Qt projects/from_local/networklib.h";
    return 0;
}
