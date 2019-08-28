#include <iostream>
#include<string>

#include "ev_handler.h"

int main()
{
    string ip="192.168.9.1";
    int port=4000;
    netaddr n_addr;
    int sock=n_addr.net_init(ip,port);
    if(sock<0){
           return -1;
    }
    ThreadPool th_pool(4);
    ev_handler ev_hdr;
    if(ev_hdr.start_ev(sock)==false){
           return -1;
    }
    
    return 0;
}
