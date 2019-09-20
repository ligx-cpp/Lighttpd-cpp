#include"network.h"

netaddr* netaddr::m_stance=NULL;//static成员的初始化要在实现中进行，不能在头文件进行
netaddr* netaddr::getInstance(){
                 if (m_stance == NULL) {
                     m_stance = new netaddr();
                 }
                 return m_stance;
        }
int  netaddr::net_init(string ip,int port){
        bzero(&name_addr,sizeof(struct sockaddr_in));
        name_addr.sin_family=AF_INET;
	name_addr.sin_port=htons(port);
        name_addr.sin_addr.s_addr=inet_addr(ip.c_str());//表示指明服务器地址
	//net_addr.sin_addr.s_addr=htonl(INADDR_ANY);//表明可接受任意IP地址
	int sock=socket(AF_INET,SOCK_STREAM,0);
	if(sock==-1){
		return -1;
	}
//------------------------------------------------------------------------------------------------------

	int reuse=1;
	setsockopt(sock,SOL_SOCKET,SO_REUSEADDR,&reuse,sizeof(reuse));//一般来说，一个端口释放后会等待两分钟之后才能再被使用，SO_REUSEADDR是让端口释放后立即就可以被再次使用,SO_REUSEADDR用于对TCP套接字处于TIME_WAIT状态下的socket，允许重复绑定使用。server程序总是应该在调用bind()之前设置SO_REUSEADDR套接字选项

//-----------------------------------------------------------------------------------------------------
	int bind_num=bind(sock,(struct sockaddr*)&name_addr,sizeof(name_addr));
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
