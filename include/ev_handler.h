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
#include<plugin.h>
#include<vector>
class ev_handler{
public:
	ev_handler(size_t thread_num,int sock_et);//size_t是一个无符号整形
	~ev_handler();
        static void sock_get(evutil_socket_t sock,short event,void* arg);
	bool start_ev();
        void set_plugin();
private:
        ThreadPool* th_pool;//线程对象
        int sock;//绑定服务器地址的套接字
        size_t thread_num;//要生成的线程数(线程数只能是正的;所以用了size_t)
	struct event_base *m_base;//监听I/O事件集合
	struct event* m_event;//专门监听I/O事件(连接事件)
        plugin plugin_t;
        std::vector<plugin*>plugin_set;//存放插件集合
};


#endif
