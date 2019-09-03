#include<rw_handler.h>


rwhand::rwhand(){
	w_event=NULL;
        r_event=NULL;
        w_base = NULL;
}

void rwhand::init(int new_fd){
        w_base=event_base_new(); 
        r_event=event_new(w_base,new_fd,EV_READ|EV_PERSIST,rwhand::r_handle,this);
        event_add(r_event,NULL);

        //event_set(r_event,new_fd,EV_READ|EV_PERSIST,rwhand::to_read,w_event);//事件初始化
        //event_base_set(w_base,r_event);//如果有多个event_base，则才需要这步；就一个event_base时，是不需要这步的，因为此时current_base就等于event_base。
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
                                event_base_loopexit(con->w_base,NULL);
				return;
			}
		}
		else if (ret == 0)
		{
                        event_base_loopexit(con->w_base,NULL);
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
                                event_base_loopexit(con->w_base,NULL);
				return;
			}
		}
		else
		{
                        con->outbuf.clear();
                        
		}
                return;

}
rwhand::~rwhand(){
        std::cout<<"rwhand被析构"<<std::endl;
        if(w_base){
                close(new_sock);
                event_free(r_event);
                event_free(w_event);
                event_base_free(w_base);
        }
}
