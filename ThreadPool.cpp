#include<ThreadPool.h>


ThreadPool::ThreadPool(size_t threads)
{
       m_Threads = new LibeventThread[threads];
       for(size_t i=0; i<threads; i++)
       {//直接初始化各个子线程的结构体
           SetupThread(&m_Threads[i]);
       }
}

void ThreadPool::SetupThread(LibeventThread *me)//最好还是传指针，传值还是太浪费资源
{
       //建立子线程中的libevent读写事件处理机制
       me->w_base = event_base_new();
       if( NULL == me->w_base )
             std::cout<<"基础事件分配失败"<<std::endl;

       //在主线程和子线程之间建立管道
       int fd[2];
       int ret = socketpair(AF_LOCAL, SOCK_STREAM, 0, fd);//socketpair常用于描述符传递的处理中,所以这里使用了这个东西;并且这是全双工的（因为fork的时候将sockets[0]和socket[1]的引用计数都加了1，所以在父子进程中将多出来的减去。close不是立刻关闭文件描述符，而只是将引用计数减一.
       if (ret == -1) {
             std::cout<<"socketpair()"<<std::endl;
             return ;
       }
       me->read_fd = fd[0];
       me->write_fd = fd[1];

       //让子线程的状态机监听管道
       //为什么要使用管道？为什么要向管道中写一个“空字符”？创建一对socketpair(也就是结构体中的read_fd和write_fd),将其中的一个通过libevent的event_add接口添加到关注的事件中,它的事件类型的是READ|PERSIST, 也就是说可读同时永不删除,而这个事件的回调函数是ThreadProcess,这个函数的功能是将当前从管道中读出的套接字数据取出并且调用rwhand(读写函数)进行回调处理.其次,外部的多线程需要往libevent添加事件时rwahdn类中的函数进行添加,此时, 往socketpair中发送一个字节的数据, 这么做的目的是为了让第一步已经添加的socketpair得到响应,此时, 由第一步,必然会触发之前注册的回调函数ThreadProcess,将这个队列中的事件取出来进行处理.
       event_set(&me->read_Event,me->read_fd,EV_READ|EV_PERSIST,ThreadPool::ThreadProcess,me);//event_set()跟event_new()功能是一样的，他们的区别在我的理解看来就是直接实例化一个类对象和用new()来实例化一个类对象的区别
       event_base_set(me->w_base, &me->read_Event);
       if (event_add(&me->read_Event, 0) == -1)
       std::cout<<"基础事件分配失败"<<std::endl;
       return ;
}

//回调函数必须是静态成员函数如果将回调函数设置成全局的这样会破坏面向对象的封装性
void ThreadPool::ThreadProcess(evutil_socket_t sock,short event,void* arg){
       LibeventThread *me = (LibeventThread*) arg;
       //先从管道中读数据
       int new_fd;//定义一个临时变量来存放从管道中读出来的套接字     
       read(me->read_fd,&new_fd,sizeof(int));//这边把套接字从管道中读出来
       me->new_fd=new_fd;//最后把这个临时的信息赋值给线程信息中一并传给事件循环
       std::cout<<"新返回的套接字"<<me->new_fd<<std::endl;

       struct bufferevent *bev;//添加新事件
       bev=bufferevent_socket_new(me->w_base,me->new_fd,BEV_OPT_CLOSE_ON_FREE);
       bufferevent_setcb(bev,read_cb,NULL,event_cb,me);//给缓冲区设置回调(写回调因为用不到所以置为空)
       //然后只需要启用bufferevent读缓冲,因为默认是不启用的
       bufferevent_enable(bev,EV_READ);
       return ;
}

void ThreadPool::event_cb(struct bufferevent *bev, short events, void *arg){
       LibeventThread *con = (LibeventThread*) arg;
       bufferevent_free(bev);
}

void ThreadPool::read_cb(struct bufferevent *bev, void* arg)
{
	LibeventThread *con = (LibeventThread*) arg;
		char buf[1024];
	        bzero(buf,sizeof(buf));
	        bufferevent_read(bev,buf,sizeof(buf));
                std::cout<<buf<<std::endl;     
	        bzero(buf,sizeof(buf));
		//con->outbuf = "HTTP/1.0 200 OK\r\nContent-type: text/plain\r\n\r\n<html>\r\n<body>\r\nhello\r\n</body>\r\n</html>";
		//struct event* w_event=event_new(con->w_base,con->new_fd,EV_WRITE|EV_PERSIST,ThreadPool::w_handle,con);

		//bufferevent_write(bev,con->outbuf,sizeof(con->outbuf));
                return ;
}

/*void ThreadPool::w_handle(evutil_socket_t new_fd, short event, void* arg)
{
                LibeventThread *con = (LibeventThread*) arg;
                int ret = write(con->new_fd,con->outbuf.c_str(),con->outbuf.size());

		if (ret == -1)
		{
			if (errno != EAGAIN && errno != EINTR)
			{
                                close(con->new_fd);
                                event_base_loopexit(con->w_base,NULL);
                                return;
			}
		}
		else
		{
                        con->outbuf.clear();
                        
		}
                return;

}*/

void ThreadPool::c_thread(size_t threads){

        std::cout<<"开启线程"<<std::endl;
	for (size_t i = 0; i < threads; ++i)          
		workers.push_back(std::thread(std::mem_fn(&ThreadPool::crt_thread),this,&m_Threads[i])); //当需要利用类成员函数(ThreadPool::crt_thread)来创建子线程时，需如下代码：thread(std::mem_fn(&ThreadPool::crt_thread), Object, args..); 这个相当于STL中内置的仿函数，可以使用调取STL容器内对象的内置函数；mem_fn函数的用法是把成员函数转为函数对象，使用对象指针或对象(引用)进行绑定
        return ;
}

void ThreadPool::crt_thread(LibeventThread* arg) {//参数是把结构体传进来;如果采用的是detach方式这里传递时最好用值传递(因为安全)我用的是join()类型的;所以我可以是传指针的形式
                std::cout<<"当前运行线程ID为："<<std::this_thread::get_id()<<std::endl;
                
                //LibeventThread *me = (LibeventThread*)arg;//获得结构体信息

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
    for(size_t i=0; i<threads; i++)
              event_base_free(m_Threads[i].w_base);
        delete [] m_Threads;
    for(std::thread &worker: workers)
        worker.join();
}
 
