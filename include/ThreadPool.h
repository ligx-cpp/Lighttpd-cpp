#ifndef THREAD_POOL_H
#define THREAD_POOL_H
 
#include <vector>
#include <memory>
#include <thread>
#include <sys/types.h>
#include <sys/socket.h>//为了使用sockpair()函数
#include <unistd.h>
#include <functional>//为了使用仿函数std::mem_fn();
#include <event.h>
#include <event2/util.h>
#include<iostream>
#include <event2/bufferevent.h>
#include <event2/buffer.h>
//#include <rw_handler.h>
#include <msg_thread.h>
class ThreadPool {
public:
    ThreadPool(size_t threads);
    void c_thread(size_t threads);
    void crt_thread(LibeventThread *argarg);//创建套接字产生的sock
    void SetupThread(LibeventThread *thread);//初始化子线程的数据
    static void read_cb(struct bufferevent *bev, void* arg);
    static void event_cb(struct bufferevent *bev, short events, void *arg);
    static void ThreadProcess(evutil_socket_t sock,short event,void* arg); 
    LibeventThread *m_Threads;  
    ~ThreadPool();
private:
    size_t threads;
    std::vector< std::thread > workers;//线程队列
};



#endif
