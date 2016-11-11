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
inline char* get_res_arr(){
	return res_arr;
};
inline void push(char *res_){
	res_arr=res_;
};
inline boost::any NetworkRes::get_res(){
    return res;
};
inline void NetworkRes::push_any(boost::any value){
    res=value;
};
inline std::map<std::string, std::string> get_header(){
	return header;
};
inline void push_header(std::map<std::string, std::string>& heade){
	header=heade;
};

