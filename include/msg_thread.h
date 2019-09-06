#ifndef MSG_THREAD_H_
#define MSG_THREAD_H_
#include<event.h>
#include<event2/util.h>
#include<string.h>
//每个子线程的线程信息
class msg_thread
{
public:
  msg_thread() {}
  ~msg_thread(){
       event_free(&r_event);
  }
  int new_fd;		//分配给每个线程的新返回的套接字
  struct event_base* w_base;	//libevent的事件集合
  struct event r_event;	//监听套接字的读事件
  int read_fd;		//套接字的接收端
  int write_fd;	        //套接字的发送端
};//子线程所必备的信息专门用一个结构体来传递
#endif
