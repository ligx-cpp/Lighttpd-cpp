#ifndef EV_HANDLE_H_
#define EV_HANDLE_H_
#include<event.h>
#include<ThreadPool.h>
#include<string>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<sys/wait.h>
#include<iostream>
#include<event2/util.h>

class ev_handler{
public:
	ev_handler(int threads,int sock);
	~ev_handler();
        static void event_handler(evutil_socket_t sock,short event,void* arg);
	bool start_ev();
private:
        int sock;
        ThreadPool *th_pool;
	struct event_base *m_base;
        struct event* m_exit_event;
	struct event* m_event;
};
#endif
