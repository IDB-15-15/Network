#ifndef LOCALLIB_H
#define LOCALLIB_H

#include <boost/any.hpp>
#include <boost/interprocess/file_mapping.hpp>
#include <boost/interprocess/mapped_region.hpp>
#include <string>
#include <utility>
#include <memory>

#include "networkres.h"

NetworkRes local(std::string local_url);

#include "locallib.cpp"

#endif // LOCALLIB_H
