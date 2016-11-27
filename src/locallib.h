#ifndef LOCALLIB_H
#define LOCALLIB_H


#include <boost/any.hpp>
#include <boost/interprocess/file_mapping.hpp>
#include <boost/interprocess/mapped_region.hpp>
#include <string>

boost::any local(std::string local_url){};

#include "locallibfoo.h"

#endif // LOCALLIB_H
