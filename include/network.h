#ifndef NETWORK_H_
#define NETWORK_H_
#include<sys/socket.h>
#include<iostream>
#include<string>
#include<event.h>
#include<arpa/inet.h>
#include<event2/util.h>
#include<string.h>
using namespace std;
class netaddr{
public:
	netaddr();
	int net_init(string ip,int port);
	~netaddr();
private:
	struct sockaddr_in net_addr;//创建网络地址结构体
};
#endif

