#include<ThreadPool.h>


ThreadPool::ThreadPool(size_t thread_num)
{
       m_Threads = new msg_thread[thread_num];//初始化要创建线程的信息
       for(size_t i=0; i<thread_num; i++)
       {
           SetupThread(&m_Threads[i]);
       }
}

void ThreadPool::SetupThread(msg_thread *me)//最好还是传指针，传值还是太浪费资源
{
       //建立子线程中的libevent读写事件处理机制
       me->w_base = event_init();//每个子线程都有单独的事件集合;如果 event_base 被设置了使用锁，那么它在多个线程中可以安全的访问。但是对 event_base 的循环只能在某个线程中执行。如果希望在多个线程中进行循环，那么应该为每一个线程创建一个 event_base

       //在主线程和子线程之间建立管道
       int fd[2];//通过(消息机制)向指定线程发送消息
       int ret = socketpair(AF_LOCAL, SOCK_STREAM, 0, fd);//socketpair常用于描述符传递的处理中,所以这里使用了这个东西;并且这是全双工的（因为fork的时候将sockets[0]和socket[1]的引用计数都加了1，所以在父子进程中将多出来的减去。close不是立刻关闭文件描述符，而只是将引用计数减一.
       if (ret == -1) {
             std::cout<<"socketpair()"<<std::endl;
             return ;
       }
       me->read_fd = fd[0];
       me->write_fd = fd[1];

       //让子线程的状态机监听管道
       event_set(&me->r_event,me->read_fd,EV_READ|EV_PERSIST,ThreadPool::ThreadProcess,me);//event_set()跟event_new()功能是一样的，他们的区别在我的理解看来就是直接实例化一个类对象和用new()来实例化一个类对象的区别
       event_base_set(me->w_base, &me->r_event);//event_base_set()函数是作为event_set()的补充，该函数重新设置ev绑定的eventbase与ev的优先级如果是通过event_base_new()创建的base，需要先调用event_set()函数先初始化event，再调用event_base_new()重新设置ev绑定的新的base
       if (event_add(&me->r_event, 0) == -1)
       std::cout<<"基础事件分配失败"<<std::endl;

       return ;
}

//回调函数必须是静态成员函数如果将回调函数设置成全局的这样会破坏面向对象的封装性
void ThreadPool::ThreadProcess(evutil_socket_t sock,short event,void* arg){
       msg_thread *me = (msg_thread*) arg;
       //先从管道中读数据
       char buf[10*1024];//定义一个临时变量来存放从管道中读出来的套接字     
       read(me->read_fd,buf,sizeof(buf));//这边把套接字从管道中读出来
       std::cout<<"新返回的套接字"<<me->new_fd<<std::endl;

       struct bufferevent *bev;//添加新事件
       bev=bufferevent_socket_new(me->w_base,me->new_fd,BEV_OPT_CLOSE_ON_FREE);//BEV_OPT_CLOSE_ON_FREE:释放bufferevent时关闭底层传输端口.这将关闭底层套接字,释放底层bufferevent等;提供给bufferevent_socket_new()的套接字务必是非阻塞模式(这在之前已经设置过了)如果想以后设置文件描述符,可以设置fd为-1.     
       bufferevent_setcb(bev,read_cb,NULL,event_cb,me);//给缓冲区设置回调(写回调因为用不到所以置为空)
       //然后只需要启用bufferevent读缓冲,因为默认是不启用的
       bufferevent_enable(bev,EV_READ);
       return ;
}

void ThreadPool::event_cb(struct bufferevent *bev, short events, void *arg){
       msg_thread *me = (msg_thread*) arg;
       bufferevent_free(bev);//如果设置了 BEV_OPT_CLOSE_ON_FREE标志,并且 bufferevent有一个套接字或者底层bufferevent作为其传输端口,则释放bufferevent将关闭这个传输端口;如果出现异常就释放事件
       std::cout<<"套接字 "<<me->new_fd<<" 关闭"<<std::endl;
       return ;
}

void ThreadPool::read_cb(struct bufferevent *bev, void* arg)
{
	        msg_thread *me = (msg_thread*) arg;
                //解析请求部分
                parser_interface parser_msg;
                for(size_t i=0;i<me->plugin_set.size();++i){
                     plugin* plugin_m=static_cast<plugin*>(me->plugin_set[i]);     
                     plugin_m->init_plugin(me,i);
		}
                char buffer[10*1024];//这是10M
	        bzero(buffer,sizeof(buffer));
	        bufferevent_read(bev,buffer,sizeof(buffer));
                std::string buf;
                buf.reserve(10*1024);
                buf=buffer;
                //std::cout<<buf<<std::endl;
                parser_msg.parser_request(me,buf);
                //这里必须能获得me解析过后的消息并把它传给动态库去执行

                for(size_t i=0;i<me->plugin_set.size();++i){
                     plugin* plugin_m=static_cast<plugin*>(me->plugin_set[i]);
                     plugin_m->ResponseStart(me,i);
                }
	        //bzero(buf,sizeof(buf));
		std::string outbuf;
		outbuf.reserve(10*1024); 
                outbuf+= (me->sponse_msg->make_response());//先写好响应头
                for(size_t i=0;i<me->plugin_set.size();++i){
                     plugin* plugin_m=static_cast<plugin*>(me->plugin_set[i]);
                     plugin_m->Write(me,i);//获得响应体
                } 
                outbuf+=(me->sponse_msg->http_body);

                bufferevent_write(bev,outbuf.c_str(),outbuf.size());
                for(size_t i=0;i<me->plugin_set.size();++i){
                     plugin* plugin_m=static_cast<plugin*>(me->plugin_set[i]);
                     plugin_m->ResponseEnd(me,i);
                }
                delete me->parsered_msg;
		me->parsered_msg = NULL;
		me->sponse_msg->reset_response();
                return ;
}

void ThreadPool::c_thread(size_t thread_num){

        std::cout<<"开启线程"<<std::endl;
	for (size_t i = 0; i < thread_num; ++i)          
		workers.push_back(std::thread(std::mem_fn(&ThreadPool::crt_thread),this,&m_Threads[i])); //当需要利用类成员函数(ThreadPool::crt_thread)来创建子线程时，需如下代码：thread(std::mem_fn(&ThreadPool::crt_thread), Object, args..); 这个相当于STL中内置的仿函数，可以使用调取STL容器内对象的内置函数；mem_fn函数的用法是把成员函数转为函数对象，使用对象指针或对象(引用)进行绑定
        return ;
}

void ThreadPool::crt_thread(msg_thread* arg) {//参数是把结构体传进来;如果采用的是detach方式这里传递时最好用值传递(因为安全)我用的是join()类型的;所以我可以是传指针的形式
                std::cout<<"当前运行线程ID为："<<std::this_thread::get_id()<<std::endl;
                event_base_dispatch(arg->w_base);//开启的线程中只能有这一句//因为这个底层是一个死循环，退出来就相当于连接已经结束了，而且这个死循环只能之只执行一次

		/*while(true)//最初想的是用条件变量的方式去获取套接字但事实上并不好用
		{			
			//C++11以后在标准库里引入了std::function模板类，这个模板概括了函数指针的概念,函数指针只能指向一个函数，而std::function对象可以代表任何可以调用的对象，比如说任何可以被当作函数一样调用的对象
			{//这个是为了说明锁的作用域
				std::unique_lock<std::mutex> lock(this->queue_mutex);
				this->condition.wait(lock,[this] { return this->stop || !this->tasks.empty(); });//在线程被阻塞时,该函数会自动调用queue_mutex.unlock() 释放锁，使得其他被阻塞在锁竞争上的线程得以继续执行;一旦当前线程获得通知(notified，通常是另外某个线程调用 notify_(one/all)唤醒了当前线程),wait()函数也是自动调用queue_mutex.lock(),使得queue_mutex的状态和wait函数被调用时相同           
				if (this->stop && this->tasks.empty())
					return;
				new_fd = this->tasks.front();
				this->tasks.pop();
			}          
		}*/
                                
                return ;
}
 
ThreadPool::~ThreadPool()
{
    delete [] m_Threads;
    for(size_t i=0; i<thread_num;++i){
        event_base_free(m_Threads->w_base);
    }
    for(std::thread &worker: workers)
        worker.join();
}
 
