#ifndef PARSER_MSG_H_
#define PARSER_MSG_H_
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"http_parser.h"
#include"http_msg.h"
class parse_msg{//基本上网上调用http_parser库的博客都一个模子刻出来的;我也是常规使用
public:
        parse_msg();
        ~parse_msg();
        int parser_request();//开始解析
        //一共七个回调;
        static int on_message_begin(http_parser *parser);//通知回调：说明开始解析HTTP消息
        static int url_cb(http_parser *parser, const char *buf, size_t len);//当解析到URL时的回调
        static int header_field_cb(http_parser *parser, const char *buf, size_t len);//当解析到头部字段名称的回调
        static int header_value_cb(http_parser *parser, const char *buf, size_t len);//当解析到头部字段值的回调
        static int on_headers_complete(http_parser *parser);//通知回调：说明HTTP报文头部解析完毕
        static int body_cb(http_parser *parser, const char *buf, size_t len);//当解析到HTTP消息主体的回调
        static int on_message_complete(http_parser *parser);//通知回调：说明消息解析完毕
private:
        http_quest_msg quest_msg;//我把http信息放在线程信息里面传入回调函数中
        http_sponse_msg sponse_msg;
        http_parser parser;//结构体对象;
        http_parser_settings	settings;//设置回调的对象
};
#endif
/*
调用过程大概如下:
http_parser_settings settings;
settings.on_url = my_url_callback;
settings.on_header_field = my_header_field_callback;
...
// 为结构体申请内存
http_parser *parser = malloc(sizeof(http_parser));
// 初始化解析器
http_parser_init(parser, HTTP_REQUEST);
// 设置保存调用者的数据，用于在callback内使用
parser->data = my_socket;
//执行解析
http_parser_execute(&parser, &settings,base, sub_buf.size());
*/
