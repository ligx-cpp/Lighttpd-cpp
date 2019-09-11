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
        evutil_make_socket_nonblocking(new_fd);//将套接字设置非阻塞模式
        int tid = (current_thread + 1) % 2;  
        msg_thread *thread = ev->th_pool->m_Threads + tid;//把获得的线程信息转存到一个临时的线程信息类里面然后发送到指定线程
        
        current_thread = tid;//更新值
        
        thread->new_fd=new_fd;
        thread->plugin_set=ev->plugin_set;//把刚才得到的插件信息发送到子线程中去
        write(thread->write_fd," ",1);//通过写事件直接把套接字传入管道中去
            
        return ;        
}

bool ev_handler::start_ev(){
        //应该在主线程中配置插件
        set_plugin();
        th_pool->c_thread(thread_num);//初始化线程
	m_base=event_base_new();
        m_event=event_new(m_base,sock,EV_READ|EV_PERSIST,ev_handler::sock_get,this);
        event_add(m_event,NULL);
	event_base_dispatch(m_base);//主线程中的循环
	return true;
}
int ev_handler::set_plugin(){
        std::string path;
	for(int i=0;i<plugin_t.path_list.size();++i){//用来获取不同插件的动态链接库(我只实现了一个)在函数运行时动态的加载
		path=plugin_t.path_list[i];
		//这里通过提供dlopen、dlsym、dlerror和dlcolose函数获取动态链接库的函数(大概就是三段式1.dlopen()2.dlsym()3.dlclose())
		void* handle=dlopen(path.c_str(),RTLD_LAZY);//在dlopen的()函数以指定模式打开指定的动态连接库文件,并返回一个句柄给调用进程: RTLD_LAZY 暂缓决定，等有需要时再解出符号 编译时候要加入-ldl (指定dl库)
		if (!handle) {
		        std::cout<<"handle为空!"<<std::endl;
			dlerror();//dlerror返回一个字符串用以描述错误
                        return -1;
		}

		plugin::SetupPlugin setup_plugin=(plugin::SetupPlugin)dlsym(handle,"SetupPlugin");//dlsym从指定的（由dlopen的返回值指定）库中获得指定的函数;;这里就开始执行插件函数了；
	        if(!setup_plugin){
			std::cout<<"setup_plugin为空！"<<std::endl;
			dlerror();
			return -1;
		}

                plugin::DestroyPlugin remove_plugin=(plugin::DestroyPlugin)dlsym(handle,"DestroyPlugin");
	        if(!remove_plugin){
			std::cout<<"remove_plugin为空！"<<std::endl;
			dlerror();
			return -1;
		}
		plugin* plugin_t=setup_plugin();//执行调用其实这里是多态的思想左边是基类指针右边是延迟调用；延迟到plugin_static.cpp和其他插件中
		plugin_t->setup_plugin=setup_plugin;//接下来是保存信息
                plugin_t->remove_plugin=remove_plugin;
		plugin_t->plugin_i=i;
                plugin_t->handle=handle;
		plugin_set.push_back(plugin_t);//plugin_set(动态链接库的集合)是个存放plugin对象的数组;每一种plugin_t对象都需要用动态链接库中的初始化函数去初始化。               
	}
        return 0;
}
ev_handler::~ev_handler(){       
        delete th_pool;
	if(m_base){
	      event_free(m_event);
	      event_base_free(m_base);
	}
        //在这里释放动态库
        for(int i=0;i<plugin_set.size();++i){
              plugin* plugin_m=static_cast<plugin*>plugin_set[i];
              plugin::DestroyPlugin remove_plugin = plugin->remove_plugin;
              remove_plugin(plugin_m);//在这里调用
              dlclose(plugin_m->handle);//关闭
        }
}
