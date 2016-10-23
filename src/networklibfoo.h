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
inline std::string NetworkRes::get_res(){
    return res;
};
inline void NetworkRes::push(std::string value){
    res=value;
};

