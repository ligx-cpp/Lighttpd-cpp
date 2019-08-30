#include<rw_handler.h>


rwhand::rwhand(){
	w_event=NULL;
        r_event=NULL;
        w_base = NULL;
}
void rw_hand::to_read(int new_fd,short event,void* arg){
        struct event_base* w_base = arg;
	//创建一个字符数组来接收客户端发来的消息
	char buf[1024];
	memset(buf,0,sizeof(buf));
	int r_size=read(new_fd,buffer,sizeof(buf));
	if(r_size==0){//说明读取关闭
		event_del(event);
		//event_base_loopexit(base,NULL);
		//event_base_free(base);
		close(new_fd);
		return ;
	}

	//要产生回显效果必须还要设置写事件
        w_event = event_new(w_base,new_fd, EV_WRITE | EV_PERSIST, do_write,w_base);


	//event_set(w_event,new_fd,EV_WRITE,rw_hand::on_write,w_write);
        //event_base_set(w_event,w_base);

        event_add(w_event,NULL);
        return ;	
}
void rw_hand::to_write(int new_fd,short event,void* arg){
	char buf[1024];
	memset(buf,0,sizeof(buf));
	string str="OKOKOKOKOKOKOKOK";
	strcpy(buf, str.c_str());
	int w_szie=write(new_fd,buf,sizeof(buf));


        event_del(w_event);
	event_base_loopexit(base,NULL);
	event_base_free(w_base);
	close(new_fd);
	return ;

}
rwhand::~rwhand(){

}
