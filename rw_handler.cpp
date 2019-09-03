#include<rw_handler.h>


rwhand::rwhand(){
        //con_event=NULL;
	w_event=NULL;
        r_event=NULL;
        w_base = NULL;
}

void rwhand::init(int new_fd){
        w_base=event_base_new(); 
        r_event=event_new(w_base,new_fd,EV_READ|EV_PERSIST,rwhand::r_handle,this);
        //event_set(r_event,new_fd,EV_READ|EV_PERSIST,rwhand::to_read,w_event);//事件初始化
        //event_base_set(w_base,r_event);//如果有多个event_base，则才需要这步；就一个event_base时，是不需要这步的，因为此时current_base就等于event_base。

        event_add(r_event,NULL);

        //创建信号事件处理器
        //w_exit_event=evsignal_new(w_base,SIGINT,ThreadPool::thread_exit,w_base);
        //evsignal_add(w_exit_event,NULL);

        //循环
        event_base_dispatch(w_base);
        return ;
}
void rwhand::r_handle(evutil_socket_t new_fd, short event, void* arg)
{
	rwhand* con = (rwhand*)arg;
        con->new_sock=new_fd;
		char buf[1024];
	        bzero(buf,sizeof(buf));
	        int ret=read(new_fd,buf,sizeof(buf));
		if (ret == -1)
		{
			if (errno != EAGAIN && errno != EINTR)
			{
				delete con;
				return;
			}
		}
		else if (ret == 0)
		{
			delete con;
			return;
		}
		else
		{
                        std::cout<<buf<<std::endl;     
			bzero(buf,sizeof(buf));
		}
		con->outbuf = "HTTP/1.0 200 OK\r\nContent-type: text/plain\r\n\r\n<html>\r\n<body>\r\nhello\r\n</body>\r\n</html>";
		con->w_event=event_new(con->w_base,new_fd,EV_WRITE,rwhand::w_handle,con);

		event_add(con->w_event, NULL);
                return ;
}
void rwhand::w_handle(evutil_socket_t new_fd, short event, void* arg)
{
                rwhand* con = (rwhand*)arg;
                int ret = write(new_fd,con->outbuf.c_str(),con->outbuf.size());

		if (ret == -1)
		{
			if (errno != EAGAIN && errno != EINTR)
			{
				delete con;
				return;
			}
		}
		else
		{
                        con->outbuf.clear();
		}
                return;

}
/*void rwhand::rw_event(evutil_socket_t new_fd, short event, void* arg)
{
	
	//	std::cout << "Want read" <<std::endl;
	//	Connection *con = (Connection*)arg;
	
	rwhand* con = (rwhand*)arg;
        con->new_sock=new_fd;
        std::cout<<"con对象的new_sock为："<<con->new_sock<<std::endl;
	if (event==EV_READ)
	{
		char buf[1024];
	        bzero(buf,sizeof(buf));
	        int ret=read(new_fd,buf,sizeof(buf));
		if (ret == -1)
		{
			if (errno != EAGAIN && errno != EINTR)
			{
				delete con;
				return;
			}
		}
		else if (ret == 0)
		{
			delete con;
			return;
		}
		else
		{
                        std::cout<<buf<<std::endl;     
			bzero(buf,sizeof(buf));
		}
		con->con_outbuf = "HTTP/1.0 200 OK\r\nContent-type: text/plain\r\n\r\n<html>\r\n<body>\r\nhello\r\n</body>\r\n</html>";
		con->NotWantRead();
                
		con->WantWrite();
	}

	if (event==EV_WRITE)
	{
		int ret = write(new_fd, con->con_outbuf.c_str(), con->con_outbuf.size());

		if (ret == -1)
		{
			if (errno != EAGAIN && errno != EINTR)
			{
				delete con;
				return;
			}
		}
		else
		{

		}
		con->NotWantWrite();
		con->WantRead();
	}
}*/
rwhand::~rwhand(){
        if(w_base){
                event_free(r_event);
                event_free(w_event);
                event_base_loopexit(w_base, NULL);//停止loop循环
                event_base_free(w_base);
        }
}
