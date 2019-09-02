#include<network.h>

netaddr::netaddr(){
        bzero(&net_addr,sizeof(net_addr));
}
int  netaddr::net_init(string &ip,int port){
        net_addr.sin_family=AF_INET;
	net_addr.sin_port=htons(port);
	net_addr.sin_addr.s_addr=inet_addr(ip.c_str());

	int sock=socket(AF_INET,SOCK_STREAM,0);
	if(sock==-1){
		return -1;
	}
//------------------------------------------------------------------------------------------------------
	
        evutil_make_socket_nonblocking(sock);//使用even2将套接字设置为非阻塞模式
	int reuse=1;
	setsockopt(sock,SOL_SOCKET,SO_REUSEADDR,&reuse,sizeof(reuse));//一般来说，一个端口释放后会等待两分钟之后才能再被使用，SO_REUSEADDR是让端口释放后立即就可以被再次使用,SO_REUSEADDR用于对TCP套接字处于TIME_WAIT状态下的socket，允许重复绑定使用。server程序总是应该在调用bind()之前设置SO_REUSEADDR套接字选项

//-------------------------------------------------------------------------------------------------------
	int bind_num=bind(sock,(struct sockaddr*)&net_addr,sizeof(net_addr));
	if(bind_num!=0){
		return -1;
	}
	int listen_num=listen(sock,5);
	if(listen_num!=0){
		return -1;
	}

	return sock;
}

netaddr::~netaddr(){
}
