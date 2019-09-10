#ifndef HTTP_MSG_H_
#define HTTP_MSG_H_
#include<string>
#include<map>
class http_quest_msg{
	std::string http_method;
	std::string http_url;
	//std::string http_version;
        std::string http_header_field;
        std::string http_header_field;
	std::string http_body;
	std::map<std::string,std::string>http_headers//请求头	
};
class http_sponse_msg{
	int http_code;
	std::string http_phrase;
	//std::string http_version;
	std::string http_body;
	std::map<std::string,std::string>http_headers//响应头
	std::string get_response();
        void reset_response();	
};

#endif
