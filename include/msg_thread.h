#ifndef MSG_THREAD_H_
#define MSG_THREAD_H_
#include<event.h>
#include<event2/util.h>
#include<string.h>
#include<vector>
#include"http_msg.h"
#include<queue>
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

  std::vector<void*>plugin_set;//存放插件集合;因为这里要避免使用plugin类所以使用了void*类型;
  std::vector<void*>plugin_data_slots;//用来存放插件中的各种结构体

  http_quest_msg *quest_msg;//把http信息放在线程信息里面传入回调函数中
  http_quest_msg *parsered_msg;//这里用指针是为了多次分配对象;
        
  http_sponse_msg *sponse_msg;
        
  std::queue<http_quest_msg> quest_queue;//制作一个请求队列;
};//子线程所必备的信息专门用一个结构体来传递

#endif
