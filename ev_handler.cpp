#include<ev_handler.h>


ev_handler::ev_handler(int threads,int sock):sock(sock){
	m_base=NULL;
        m_exit_event=NULL;
        m_event=NULL;
        th_pool=new ThreadPool(threads);
}

void ev_handler::event_handler(evutil_socket_t sock_et,short event,void* arg){
        ev_handler* ev=(ev_handler*)arg;
        struct sockaddr_in temp_addr;
        int addrlen=sizeof(temp_addr);//用来存放连接上来的客户端地址
        int new_fd = accept(sock_et,(struct sockaddr*) &temp_addr,(socklen_t*)&addrlen);//如果线程池已用完，怎么办呢?
        if(new_fd < 0){//连接客户端所返回的套接字
             std::cout<<"Accept error in on_accept()"<<std::endl;
             return;
        }

        ev->th_pool->enqueue(new_fd);
        
        
        //将套接字写入到子线程当中去//socketpair()函数虽然实现了双向通讯的功能，但是也存在一些限制：比如只能在一个进程的线程之间进行双向通讯或者是在具有亲缘关系的进程间进行通讯,所以本次实现不打算用socketpair实现线程间的通信
        return ;
        
}
bool ev_handler::start_ev(){     
	m_base=event_base_new();
        m_event=event_new(m_base,sock,EV_READ|EV_PERSIST,ev_handler::event_handler,this);
        //event_set(m_event,sock,EV_READ|EV_PERSIST,ev_handler::event_handler,this);//初始化listen_ev事件
        //event_base_set(m_base, listen_ev);//event_base_set()函数是作为event_set()的补充，该函数重新设置ev绑定的eventbase与ev的优先级如果是通过event_base_new()创建的base，需要先调用event_set()函数先初始化event，再调用event_base_new()重新设置ev绑定的新的base
        event_add(m_event,NULL);//这里没有创建通用事件处理器？？？？
        //m_exit_event=evsignal_new(m_base,SIGINT,ev_handler::ev_exit,m_base);//中断信号
        //evsignal_add(m_exit_event,NULL);
	event_base_dispatch(m_base);
	return true;
}

/*void ev_exit(evutil_socket_t sig,short event,void* arg){
        event_base_loopexit((struct event_base*)arg,NULL);
        return ;
}*/

ev_handler::~ev_handler(){
        delete th_pool;
	if(m_base){
	      event_free(m_exit_event);
	      event_base_free(m_base);
	}
}
