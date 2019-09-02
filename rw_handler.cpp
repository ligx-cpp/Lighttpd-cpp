#include<rw_handler.h>


rwhand::rwhand(){
        con_event=NULL;
	//w_event=NULL;
        //r_event=NULL;
        w_base = NULL;
}
void rwhand::WantRead()
{
	//m_want_read = true;
	short event = event_get_events(con_event);
	event_del(con_event);
	event_assign(con_event, w_base, new_sock, event | EV_READ, rwhand::rw_event, this);
	event_add(con_event, NULL);
}

void rwhand::NotWantRead()
{
	//m_want_read = false;
	short event = event_get_events(con_event);
	event_del(con_event);
	event_assign(con_event, w_base, new_sock, event & (~EV_READ), rwhand::rw_event, this);
	event_add(con_event, NULL);
}

void rwhand::WantWrite()
{
	short event = event_get_events(con_event);
	event_del(con_event);
	event_assign(con_event, w_base, new_sock, event | EV_WRITE, rwhand::rw_event, this);
	event_add(con_event, NULL);
}

void rwhand::NotWantWrite()
{
	short event = event_get_events(con_event);
	event_del(con_event);
	event_assign(con_event, w_base, new_sock, event & (~EV_WRITE), rwhand::rw_event, this);
	event_add(con_event, NULL);
}
void rwhand::rw_event(evutil_socket_t new_fd, short event, void* arg)
{
	/*
		std::cout << "Want read" <<std::endl;
		Connection *con = (Connection*)arg;
	*/
	rwhand* con = (rwhand*)arg;
        con->new_sock=new_fd;
	if (event & EV_READ)
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
			bzero(buf,sizeof(buf));
		}
		con->con_outbuf = "HTTP/1.0 200 OK\r\nContent-type: text/plain\r\n\r\n<html>\r\n<body>\r\nhello\r\n</body>\r\n</html>";
		con->NotWantRead();
		con->WantWrite();
	}

	if (event & EV_WRITE)
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
}
rwhand::~rwhand(){

}
