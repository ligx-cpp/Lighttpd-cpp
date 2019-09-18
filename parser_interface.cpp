#include<parser_interface.h>

parser_interface::parser_interface(){
     bzero(&settings, sizeof(settings));   
    settings.on_message_begin    = On_message_begin;
    settings.on_url              = Url_cb; 
    settings.on_header_field     = Header_field_cb;
    settings.on_header_value     = Header_value_cb;
    settings.on_headers_complete = On_headers_complete;
    settings.on_body             = Body_cb;
    settings.on_message_complete = On_message_complete;
     http_parser_init(parser,HTTP_REQUEST);//初始化解析器
     
}

int parser_interface::parser_request(http_quest_msg* mes,char* buffer,int buffer_num){//把要解析的字符窗传递进来;还有刚开始信息
     
     parser->data=mes;//像之前的event回调函数传递对象的方式来传递对象
     http_parser_execute(parser,&settings,buffer,buffer_num);//buffer_num这里卡了一天----------------------------------------------------------------------------------
     return 0;
}

int parser_interface::On_message_begin(http_parser *parser){
     std::cout<<"开始解析消息"<<std::endl;
     return 0;
}

int parser_interface::Url_cb(http_parser *parser, const char *buf, size_t len){
     http_quest_msg* mes=(http_quest_msg*)(parser->data);//通过这种方式将对对象传来因为回调函数的参数是固定形式的
     mes->url = (char*)malloc(len + 1);
     memcpy(mes->url, buf, len);
     mes->url[len] = '\0';
     return 0;
}

int parser_interface::Header_field_cb(http_parser *parser, const char *buf, size_t len){
     http_quest_msg* mes=static_cast<http_quest_msg*>(parser->data);
     mes->name = (char*)malloc(len + 1);
     memcpy(mes->name, buf, len);
     mes->name[len] = '\0';
     return 0;
}

int parser_interface::Header_value_cb(http_parser *parser, const char *buf, size_t len){
     http_quest_msg* mes=static_cast<http_quest_msg*>(parser->data);
     mes->value = (char*)malloc(len + 1);
     memcpy(mes->value, buf, len);
     mes->value[len] = '\0';
     mes->http_headers[mes->name]=mes->value;//把“名值对”用map匹配起来
     return 0;
}

int parser_interface::On_headers_complete(http_parser *parser){
     http_quest_msg* mes=static_cast<http_quest_msg*>(parser->data);
     mes->method = (char *)http_method_str((http_method)parser->method);//所用的方法
     return 0;
}

int parser_interface::Body_cb(http_parser *parser, const char *buf, size_t len){
     http_quest_msg* mes=static_cast<http_quest_msg*>(parser->data);
     mes->body = (char*)malloc(len + 1);
     memcpy(mes->body, buf, len);
     mes->body[len] = '\0';
     return 0;
}

int parser_interface::On_message_complete(http_parser *parser){
     std::cout<<"解析完毕！"<<std::endl;
     return 0;
}

parser_interface::~parser_interface(){
    
}
