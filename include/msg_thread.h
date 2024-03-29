#ifndef MSG_THREAD_H_
#define MSG_THREAD_H_
#include<event.h>
#include<event2/util.h>
#include<string.h>
#include<vector>
#include"http_msg.h"
#include<queue>
#include "file_data.h"
//每个子线程的线程信息
class msg_thread
{
public:
  msg_thread() {
  }
  ~msg_thread(){
       event_free(&r_event);
  }
  int new_fd;		//分配给每个线程的新返回的套接字
  struct event_base* w_base;	//libevent的事件集合
  struct event r_event;	//专门监听管道中的事件是否出现
  int read_fd;		//套接字的接收端
  int write_fd;	        //套接字的发送端
  std::string outbuf;//发送数据用的字符串
  
  std::vector<void*>plugin_set;//存放插件派生类集合;因为这里派生类各不相同所以使用了void*类型;
  std::vector<void*>plugin_file_data;//用来存放各种插件中的独有数据所以这里要用void*类型

  http_sponse_msg sponse_msg;
  http_quest_msg parsered_msg;//这里用指针是为了多次分配对象;
        
  std::queue<http_quest_msg*> quest_queue;//制作一个请求队列;
};//子线程所必备的信息专门用一个结构体来传递

#endif
