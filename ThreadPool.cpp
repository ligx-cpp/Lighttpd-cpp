#include<ThreadPool.h>


ThreadPool::ThreadPool(int threads)
	: stop(false)
{
        w_exit_event=NULL;
        r_event=NULL;
	for (size_t i = 0; i < threads; ++i)
		workers.emplace_back(std::thread(std::mem_fn(&ThreadPool::crt_thread),this)); //当需要利用类成员函数(ThreadPool::crt_thread)来创建子线程时，需如下码码：thread(std::mem_fn(&ThreadPool::crt_thread), Object, args..); 这个相当于STL中内置的仿函数，可以使用调取STL容器内对象的内置函数；mem_fn函数的用法是把成员函数转为函数对象，使用对象指针或对象(引用)进行绑定
}

void ThreadPool::crt_thread() {
                //初始化base
                struct event_base* w_base=event_base_new();
                rwhand rw_ev;
		for (;;)
		{
			int new_fd=0;
			//C++11以后在标准库里引入了std::function模板类，这个模板概括了函数指针的概念,函数指针只能指向一个函数，而std::function对象可以代表任何可以调用的对象，比如说任何可以被当作函数一样调用的对象
			{//这个是为了说明锁的作用域
				std::unique_lock<std::mutex> lock(this->queue_mutex);
				this->condition.wait(lock,
					[this] { return this->stop || !this->tasks.empty(); });//在线程被阻塞时,该函数会自动调用queue_mutex.unlock() 释放锁，使得其他被阻塞在锁竞争上的线程得以继续执行;一旦当前线程获得通知(notified，通常是另外某个线程调用 notify_(one/all)唤醒了当前线程),wait()函数也是自动调用queue_mutex.lock(),使得queue_mutex的状态和wait函数被调用时相同           
				if (this->stop && this->tasks.empty())
					return;
				new_fd = this->tasks.front();
				this->tasks.pop();
			}

			evutil_make_socket_nonblocking(new_fd);

                        //这里要加入读事件通过回调函数
                        rw_ev.r_event=event_new(w_base,new_fd,EV_READ|EV_PERSIST,rwhand::to_read,w_base);

                        //event_set(r_event,new_fd,EV_READ|EV_PERSIST,rwhand::to_read,w_event);//事件初始化
                        //event_base_set(w_base,r_event);//如果有多个event_base，则才需要这步；就一个event_base时，是不需要这步的，因为此时current_base就等于event_base。

                        event_add(rw_ev.r_event,NULL);

                        //创建信号事件处理器
                        w_exit_event=evsignal_new(w_base,SIGINT,Thread_Pool::thread_exit,w_base);
                        evsignal_add(w_exit_event,NULL);

                        //循环
                        event_base_dispatch(w_base); 
		}
                return ;
}


void ThreadPool::enqueue(int new_fd)
{
        
    {
        std::unique_lock<std::mutex> lock(queue_mutex);
 
        // don't allow enqueueing after stopping the pool
        if(stop)
            throw std::runtime_error("enqueue on stopped ThreadPool");
 
        tasks.push(new_fd);//把任务压入队列
    }
}
    condition.notify_one();//notify是通知一个线程获取锁，notifyAll是通知所有相关的线程去竞争锁;notify_one()(随机唤醒一个等待的线程)和notify_all()(唤醒所有等待的线程)//因为任务队列里有了任务,所以
    return ;
}

void ThreadPool::thread_exit(evutil_socket_t sig,short event,void* arg){
                event_base_loopexit((struct event_base*)arg,NULL);
                return ;


 
 
ThreadPool::~ThreadPool()
{
    {
        std::unique_lock<std::mutex> lock(queue_mutex);
        stop = true;//stop变成了true 此时通知所有线程
    }
    condition.notify_all();//等待所有的线程任务执行完成退出
    for(std::thread &worker: workers)
        worker.join();
}
 
