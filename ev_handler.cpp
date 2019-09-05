#include<ev_handler.h>


ev_handler::ev_handler(size_t thread,int sock_et):threads(thread),sock(sock_et){
	m_base=NULL;
        m_event=NULL;
        th_pool=new ThreadPool(threads);
}

void ev_handler::event_handler(evutil_socket_t sock_et,short event,void* arg){
        int last_thread=0;
        ev_handler* ev=(ev_handler*)arg;
        struct sockaddr_in temp_addr;
        int addrlen=sizeof(temp_addr);//用来存放连接上来的客户端地址
        int new_fd = accept(sock_et,(struct sockaddr*) &temp_addr,(socklen_t*)&addrlen);//如果线程池已用完，怎么办呢?
        if(new_fd < 0){//连接客户端所返回的套接字
             std::cout<<"Accept error in on_accept()"<<std::endl;
             return;
        }
        int tid = (last_thread + 1) % 2;        //memcached中线程负载均衡算法
 
        LibeventThread *thread = ev->th_pool->m_Threads + tid;
 
        last_thread = tid;
        ev->th_pool->m_Threads[0].new_fd=new_fd;
        write(thread->write_fd,&new_fd,sizeof(int));//通过写事件直接把套接字传入管道中去
        std::cout<<"连接成功"<<std::endl;       
        return ;        
}

bool ev_handler::start_ev(){
        th_pool->c_thread(threads);   
	m_base=event_base_new();
        m_event=event_new(m_base,sock,EV_READ|EV_PERSIST,ev_handler::event_handler,this);

        //event_set(m_event,sock,EV_READ|EV_PERSIST,ev_handler::event_handler,this);//初始化listen_ev事件
        //event_base_set(m_base, listen_ev);//event_base_set()函数是作为event_set()的补充，该函数重新设置ev绑定的eventbase与ev的优先级如果是通过event_base_new()创建的base，需要先调用event_set()函数先初始化event，再调用event_base_new()重新设置ev绑定的新的base
        event_add(m_event,NULL);//这里没有创建通用事件处理器？？？？
	event_base_dispatch(m_base);
	return true;
}

ev_handler::~ev_handler(){
        
        delete th_pool;
	if(m_base){
	      event_free(m_event);
	      event_base_free(m_base);
	}
}
