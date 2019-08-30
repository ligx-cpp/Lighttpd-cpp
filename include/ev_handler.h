#ifndef EV_HANDLE_H_
#define EV_HANDLE_H_
#include<event.h>
#include<ThreadPool.h>
#include<string>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<iostream>

class ev_handler{
public:
	ev_handler(int threads,int sock);
	~ev_handler();
        void event_handler(evutil_socket_t sock,short event,void* arg);
        static void ev_exit(evutil_socket_t sig,short event,void* arg);//libevent定义evutil_socket_t类型为(int)整型指针，可以处理socket()或者accept()的输出，而没有指针截断的风险
	bool start_ev();
private:
        int sock;
        ThreadPool *th_pool;
	struct event_base *m_base;
        struct event* m_exit_event;
	struct event* m_event;
        struct sockaddr_in temp_addr;
};
#endif
