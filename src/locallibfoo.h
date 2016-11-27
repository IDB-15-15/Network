boost::any local(std::string local_url){
    using namespace boost::interprocess;
    const char *file_name = local_url.c_str();

    file_mapping m_file(file_name, read_only);
    mapped_region region(m_file, read_only);

    void * addr       = region.get_address();
    std::size_t size  = region.get_size();

    return 0;
};