#include <iostream>
#include<string>
#include<network.h>
#include "ev_handler.h"

int main()
{
    std::string ip="192.168.9.1";
    int port=4000;
    netaddr n_addr;
    int sock=n_addr.net_init(ip,port);
    if(sock<0){
           return -1;
    }
    ev_handler ev_hdr(4,sock);//输入要创建的线程树和绑定的套接字
    if(ev_hdr.start_ev()==false){
           return -1;
    }
    
    return 0;
}
