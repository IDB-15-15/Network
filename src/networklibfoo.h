inline int NetworkRes::get_error(){
    return error;
};
inline void NetworkRes::set_error(int err){
    error=err;
};
inline int NetworkRes::get_mode(){
    return mode;
};
inline void NetworkRes::set_mode(int mod){
    mode=mod;
};
inline const char* NetworkRes::get_res_arr(){
	return res_arr;
};
inline void NetworkRes::push(const char *res_){
	res_arr=res_;
};
inline boost::any NetworkRes::get_res(){
    return res;
};
inline void NetworkRes::push_any(boost::any value){
    res=value;
};
inline std::map<std::string, std::string> NetworkRes::get_header(){
	return header;
};
inline void NetworkRes::push_header(std::map<std::string, std::string> heade){
	header=heade;
};


