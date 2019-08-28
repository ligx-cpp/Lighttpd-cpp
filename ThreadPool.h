#ifndef THREAD_POOL_H
#define THREAD_POOL_H
 
#include <vector>
#include <queue>
#include <memory>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <future>
#include <functional>
#include <stdexcept>
#include <event.h>
#include <network.h> 
class ThreadPool {
public:
    ThreadPool(int threads);
    void crt_thread();
    static void thread_exit(evutil_socket_t sig,short event,void* arg);
    
    ~ThreadPool();
private:
    std::vector< std::thread > workers;//工作线程
    std::queue< std::function<void()> > tasks;//任务队列
    
    std::mutex queue_mutex;//同步
    std::condition_variable condition;
    bool stop;
    //事件
    netaddr ev_net;
    struct event_base* w_base;
    struct event* w_event;
    struct event* w_exit_event;
};
#endif
