#include<ev_handler.h>


ev_handler::ev_handler(size_t thread_num,int sock_et):thread_num(thread_num),sock(sock_et){
	m_base=NULL;
        m_event=NULL;
        th_pool=new ThreadPool(thread_num);//在堆上实例化对象
}

void ev_handler::sock_get(evutil_socket_t sock_et,short event,void* arg){
        int current_thread=0;
        ev_handler* ev=(ev_handler*)arg;
        struct sockaddr_in temp_addr;
        int addrlen=sizeof(temp_addr);//用来存放连接上来的客户端地址
        int new_fd = accept(sock_et,(struct sockaddr*) &temp_addr,(socklen_t*)&addrlen);
        if(new_fd < 0){//连接客户端所返回的套接字
             std::cout<<"获取客户端返回套接字失败"<<std::endl;
             return;
        }
        std::cout<<"连接成功"<<std::endl;  

        int tid = (current_thread + 1) % 2; 
         
        msg_thread *thread = ev->th_pool->m_Threads + tid;//把获得的线程信息转存到一个临时的线程信息类里面然后发送到指定线程
 
        current_thread = tid;//更新值
 
        thread->new_fd=new_fd;
 
        write(thread->write_fd,&new_fd,sizeof(int));//通过写事件直接把套接字传入管道中去
            
        return ;        
}

bool ev_handler::start_ev(){
        th_pool->c_thread(thread_num);//初始化线程
	m_base=event_base_new();
        m_event=event_new(m_base,sock,EV_READ|EV_PERSIST,ev_handler::sock_get,this);
        event_add(m_event,NULL);
	event_base_dispatch(m_base);//主线程中的循环
	return true;
}

ev_handler::~ev_handler(){       
        delete th_pool;
	if(m_base){
	      event_free(m_event);
	      event_base_free(m_base);
	}
}
