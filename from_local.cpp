#include <boost/any.hpp>
#include <boost/interprocess/file_mapping.hpp>
#include <boost/interprocess/mapped_region.hpp>
#include <iostream>

using namespace std;

int main(){
    using namespace boost::interprocess;

    char *name;
    cout << "n = ";
    cin >> name;
    const char *file_name = name;//("C:/Users/Nekit/Documents/file.txt");
    //const std::size_t file_size = 10000;

    file_mapping m_file(file_name, read_only);
    mapped_region region(m_file, read_only);

    //Get the address of the mapped region
    void * addr       = region.get_address();
    std::size_t size  = region.get_size();

    cout.write(static_cast<const char *> (addr), size);
    cout<<"ok";
    return 0;
}
