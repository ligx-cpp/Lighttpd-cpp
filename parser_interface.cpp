#include<parser_interface.h>

parser_interface::parser_interface(){
     bzero(&settings,sizeof(settings));
     settings.on_message_begin    = on_message_begin;
     settings.on_url              = url_cb;//url
     settings.on_header_field     = header_field_cb;//名称
     settings.on_header_value     = header_value_cb;//值
     settings.on_headers_complete = on_headers_complete;
     settings.on_body             = body_cb;//消息体
     settings.on_message_complete = on_message_complete;

     http_parser_init(parser,HTTP_REQUEST);//初始化解析器
}

int parser_interface::parser_request(msg_thread *me,std::string& sub_buf){//把要解析的字符窗传递进来;还有刚开始信息
     parser->data=me;//像之前的event回调函数传递对象的方式来传递对象
     http_parser_execute(parser,&settings,sub_buf.c_str(),sub_buf.size());
     return 0;
}

int parser_interface::on_message_begin(http_parser *parser){
     //开始解析消息
     msg_thread* me=static_cast<msg_thread*>(parser->data);//通过这种方式将对对象传来因为回调函数的参数是固定形式的
     me->quest_msg=new http_quest_msg();//在开始解析时为每一次解析分配一个对象
     return 0;
}

int parser_interface::url_cb(http_parser *parser, const char *buf, size_t len){
     msg_thread* me=static_cast<msg_thread*>(parser->data);
     me->quest_msg->http_url.assign(buf, len);//用c字符串buf开始len个字符赋值    
     return 0;
}

int parser_interface::header_field_cb(http_parser *parser, const char *buf, size_t len){
     msg_thread* me=static_cast<msg_thread*>(parser->data);
     me->quest_msg->http_header_field.assign(buf, len);
     return 0;
}

int parser_interface::header_value_cb(http_parser *parser, const char *buf, size_t len){
     msg_thread* me=static_cast<msg_thread*>(parser->data);
     http_quest_msg* request=me->quest_msg;
     request->http_headers[request->http_header_field]=std::string(buf,len);//把“名值对”用map匹配起来
     return 0;
}

int parser_interface::on_headers_complete(http_parser *parser){
     msg_thread* me=static_cast<msg_thread*>(parser->data);
     http_quest_msg* request=me->quest_msg;
     request->http_method = (char *)http_method_str((http_method)parser->method);//所用的方法
     return 0;
}

int parser_interface::body_cb(http_parser *parser, const char *buf, size_t len){
     msg_thread* me=static_cast<msg_thread*>(parser->data);
     me->quest_msg->http_body.assign(buf, len);
     return 0;
}

int parser_interface::on_message_complete(http_parser *parser){
     msg_thread* me=static_cast<msg_thread*>(parser->data);
     http_quest_msg *request = me->quest_msg;
     me->parsered_msg=request;
     me->quest_queue.push(request);
     me->quest_msg = NULL;
     return 0;
}

parser_interface::~parser_interface(){
     
}
