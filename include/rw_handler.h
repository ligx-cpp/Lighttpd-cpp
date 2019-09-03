#ifndef RW_HANDLER_H_
#define RW_HANDLER_H_
#include<iostream>
#include<string.h>
#include<string>
#include<event.h>
#include<sys/socket.h>
#include<unistd.h>
#include<event2/util.h>
class rwhand {
public:
	rwhand();
        void init(int new_fd);
	static void r_handle(evutil_socket_t new_fd,short event,void* arg);
        static void w_handle(evutil_socket_t new_fd,short event,void* arg);
	~rwhand();
        //struct event* con_event;//这里用两个event，一个注册读，一个注册写会效率高点
        int new_sock;
	struct event* w_event;//创建写事件
        struct event* r_event;//创建读事件
        struct event_base* w_base;
        std::string outbuf;
};
#endif
