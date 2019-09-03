#include <iostream>
#include<string>
#include<network.h>
#include "ev_handler.h"

int main()
{
    int port=4000;
    std::string ip="192.168.56.101";
    std::cout<<"请输入开放端口号：";
    //std::cin>>port;
    netaddr n_addr;
    int sock=n_addr.net_init(ip,port);
    if(sock<0){
           std::cout<<"套接字创建失败！"<<std::endl;
           return -1;
    }
    ev_handler ev_hdr(4,sock);//输入要创建的线程树和绑定的套接字
    if(ev_hdr.start_ev()==false){
           std::cout<<"程序启动失败！"<<std::endl;
           return -1;
    }
    
    return 0;
}
