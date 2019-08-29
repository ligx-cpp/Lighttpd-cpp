#include<ev_handle.h>


ev_handler::ev_handler(int threads,int sock):sock(sock){
	m_base=NULL;
        m_exit_event=NULL;
        listen_ev=NULL;
        th_pool=new ThreadPool(threads,sock);
}

void ev_handler::event_handler(int sock,short event,void* arg){
        int addrlen=sizeof(remote_addr);//用来存放连接上来的客户端地址
        int new_fd = accept(sock,  (struct sockaddr*) &remote_addr, (socklen_t*)&addrlen);//如果线程池已用完，怎么办呢?
        if(new_fd < 0){//连接客户端所返回的套接字
             std::cout<<"Accept error in on_accept()"<<std::endl;
             return;
        }
	//这里想用std::promise将套接字包装起来传给子线程
        //std::promise<int> temp;
        //temp.set_value(result);//用promise的类模板中的成员函数set_value()保存这个值

        th_pool->enqueue(new_fd);
        
        
        //将套接字写入到子线程当中去//socketpair本身虽然实现了双向通讯的功能，但是也存在一些限制：比如只能在一个进程的线程之间进行双向通讯或者是在具有亲缘关系的进程间进行通讯,所以本次实现不打算用socketpair实现线程间的通信
        return ;
        
}
bool ev_handler::start_ev(){     
	m_base=event_base_new();
        event_set(listen_ev,sock,EV_READ|EV_PERSIST,ev_handler::event_handler,NULL);//初始化listen_ev事件
        event_base_set(m_base, listen_ev);//event_base_set()函数是作为event_set()的补充，该函数重新设置event绑定的event_base与event的优先级;如果是通过event_base_new()创建的base，需要先调用event_set()函数先初始化event，再调用event_base_new()重新设置event绑定的新的base
        event_add(listen_ev,NULL);//这里没有创建通用事件处理器？？？？
        m_exit_event=evsignal_new(m_base,SIGINT,ev_handler::ev_exit,m_base);//中断信号
        evsignal_add(m_exit_event,NULL);
	event_base_dispatch(m_base);
	return true;
}

void ev_exit(evutil_socket_t sig,short event,void* arg){
        event_base_loopexit((struct event_base*)arg,NULL);
        return ;
}

ev_handler::~ev_handler(){
        delete th_pool;
	if(m_base){
	      event_free(m_exit_event);
	      event_base_free(m_base);
	}
}