#ifndef LOCALLIB_H
#define LOCALLIB_H

#include <boost/any.hpp>
#include <boost/interprocess/file_mapping.hpp>
#include <boost/interprocess/mapped_region.hpp>
#include <string>
#include <utility>
#include <memory>

#include "networkres.h"

namespace Network{

NetworkRes get_local_file(std::string local_url);
bool path(char* p);
NetworkRes directory(std::string path);
  
}

#endif // LOCALLIB_H
