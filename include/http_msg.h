#ifndef HTTP_MSG_H_
#define HTTP_MSG_H_
#include<string>
#include<map>
#include<sstream>
#include <iostream>
class http_quest_msg{
public:
	std::string http_method;
	std::string http_url;
        std::string http_header_field;
	std::string http_body;
	std::map<std::string,std::string>http_headers;//请求头
        std::string get_request(){
             
        }	
};
class http_sponse_msg{
public:
	int http_code;//状态码：100,200,300,等
	std::string http_code_des;//状态码描述
	std::string http_body;
	std::map<std::string,std::string>http_headers;//响应头
	std::string make_response(){
              //我可以利用流将一个任意的类型转换到特定的目标类型;(很方便)
              std::stringstream oss;//将要发送的消息输入到统一的输入输出流中
              oss<<"HTTP/1.1" <<" "<<http_code<<" "<< http_code_des<<"\r\n";//状态行;把值传递到流中;这里默认开启长连接;在HTTP/1.1里，就默认是开启了keep-alive

              std::map<std::string,std::string>::iterator iter=http_headers.begin();
              while (iter!=http_headers.end())//这些"名值对"都是响应头部
              {
                     oss<<iter->first<<":"<<iter->second<< "\r\n";//名值对
                     ++iter;
              }


              oss<<"Content-Length: "<<http_body.size() << "\r\n\r\n";//注意这里有个空行不能少
              oss<< http_body;//响应体可以为任何类型的数据
              return oss.str();//使用str()成员函数来获取流内部缓冲的一份拷贝
        }

        void reset_response(){
              http_code = 200;
              http_code_des = "OK";
              http_body.clear();
              http_headers.clear();//如果打算在多次转换中使用同一个stringstream对象，记住再每次转换前要使用clear()方法；在多次转换中重复使用同一个stringstream（而不是每次都创建一个新的对象）对象最大的好处在于效率。stringstream对象的构造和析构函数通常是非常耗费CPU时间的。
              return ;
        }
        
};

#endif
