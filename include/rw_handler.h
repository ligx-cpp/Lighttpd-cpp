#ifndef RW_HANDLER_H_
#define RW_HANDLER_H_
#include<iostream>

class rwhand {
public:
	rwhand();
	void to_read(int new_fd,short event,void* arg);
	void to_write(int new_fd,short event,void* arg);
	~rwhand();
private:
	struct event* w_event;//创建写事件
        struct event* r_event;//创建读事件
        struct event_base* w_base;
};
#endif
