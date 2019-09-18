#include<ev_handler.h>

int ev_handler::current_thread=0;//静态成员变量必须在类内声明;类外初始化
ev_handler::ev_handler(size_t thread_num,int sock_et):thread_num(thread_num),sock(sock_et){
	m_base=NULL;
        m_event=NULL;
        th_pool=new ThreadPool(thread_num);//在堆上实例化对象
}

void ev_handler::sock_get(evutil_socket_t sock,short event,void* arg){      
        ev_handler* ev=(ev_handler*)arg;
        struct sockaddr_in temp_addr;
        int addrlen=sizeof(temp_addr);//用来存放连接上来的客户端地址
        evutil_make_socket_nonblocking(sock);
        int new_fd = accept(sock,(struct sockaddr*) &temp_addr,(socklen_t*)&addrlen);
        if(new_fd < 0){//连接客户端所返回的套接字
             std::cout<<"获取客户端返回套接字失败"<<std::endl;
             return;
        }
        std::cout<<"连接成功"<<std::endl;
        std::cout<<"新返回的套接字为: "<<new_fd<<std::endl;
        int tid = (ev_handler::current_thread + 1) % 2;  
        msg_thread *thread = ev->th_pool->m_Threads + tid;//把获得的线程信息转存到一个临时的线程信息类里面然后发送到指定线程
        ev_handler::current_thread = tid;//更新值这里有问题
        std::cout<<ev_handler::current_thread<<std::endl;
        thread->plugin_set=ev->plugin_set;
        thread->new_fd=new_fd;
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
	for(int i=0;i<p_plugin.path_list.size();++i){//用来获取不同插件的动态链接库(我只实现了一个)在函数运行时动态的加载
		path=p_plugin.path_list[i];
		//这里通过提供dlopen、dlsym、dlerror和dlcolose函数获取动态链接库的函数(大概就是三段式1.dlopen()2.dlsym()3.dlclose())
		void* handle=dlopen(path.c_str(),RTLD_LAZY);//在dlopen的()函数以指定模式打开指定的动态连接库文件,并返回一个句柄给调用进程: RTLD_LAZY 暂缓决定，等有需要时再解出符号 编译时候要加入-ldl (指定dl库)
		if (handle==NULL) {
		        std::cout<<"handle为空!"<<std::endl;
			dlerror();//dlerror返回一个字符串用以描述错误
                        return -1;
		}

		plugin::SetupPlugin setup_plugin=(plugin::SetupPlugin)dlsym(handle,"SetupPlugin");//dlsym从指定的（由dlopen的返回值指定）库中获得指定的函数;;这里就开始执行插件函数了;函数返回值是void*,指向函数的地址，供调用使用
	        if(!setup_plugin){
			std::cout<<"setup_plugin为空！"<<std::endl;
			dlerror();
			return -1;
		}

                plugin::Destroyplugin remove_plugin=(plugin::Destroyplugin)dlsym(handle,"DestroyPlugin");
	        if(!remove_plugin){
			std::cout<<"remove_plugin为空！"<<std::endl;
			dlerror();
			return -1;
		}
		plugin* plugin_t=setup_plugin();//这里执行extern "C"第一个调用;这种方式基于C++的多态性;左边是基类指针右边是延迟调用；延迟到plugin_static.cpp和其他插件中这里很像是工厂方法模式

                plugin_t->remove_plugin=remove_plugin;//接下来是保存之后会用到的信息
		plugin_t->plugin_i=i;
                plugin_t->handle=handle;
		plugin_set.push_back(plugin_t);//plugin_set(动态链接库的集合)是个存放plugin对象的数组;每一种plugin_t对象都需要用动态链接库中的初始化函数去初始化。  这个plugin_set集合只是个临时存放链接库集合的地方;在回调函数中sock_get()中会被转存到msg_thread类中的plugin_set集合中  
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
              plugin* plugin_m=static_cast<plugin*>(plugin_set[i]);
              plugin::Destroyplugin remove_plugin = plugin_m->remove_plugin;
              remove_plugin(plugin_m);//在这里执行extern "C"中第二个函数调用
              dlclose(plugin_m->handle);//关闭
        }
}
