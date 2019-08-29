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

class ThreadPool {
public:
    ThreadPool(int threads,int sock);
    void crt_thread();//创建套接字产生的sock
    void enqueue(int new_fd);//accept的返回的新的new_fd
    static void thread_exit(evutil_socket_t sig,short event,void* arg);
    
    ~ThreadPool();
private:
    std::vector< std::thread > workers;//工作线程
    std::queue<int> tasks;//任务队列
    //同步
    std::mutex queue_mutex;
    std::condition_variable condition;
    bool stop;
    //事件
    int sock;
    struct event_base* w_base;
    struct event* w_event;
    struct event* w_exit_event;
};
#endif
