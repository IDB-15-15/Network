NetworkRes local(std::string local_url)
{
    using namespace boost::interprocess;
    const char *file_name = local_url.c_str();	//получили имя файла

    file_mapping m_file(file_name, read_only);  //прочли файл
    mapped_region region(m_file, read_only);    //запихали его в регион

    NetworkRes res;                             //создали объект класса запоминающего результат

    res.res_arr = (static_cast<const char *> (region.get_address()));   //запомнили адрес массива
    res.size = region.get_size();                                       //запомнили размер
    res.res = std::make_shared<std::pair<file_mapping, mapped_region>>
            (std::move (m_file), std::move (region));                   //сохранили сам массив

    return res;
}