#ifdef NETWORK_H_
#define NETWORK_H_
#include<sys/socket.h>
#include<string>
#include<event2/util.h>
#include<memory.h>
using namespace std;
class netaddr{
public:
	netaddr();
	int net_init(const string &ip,const int port);
	~netaddr();
private:
	int connect;
	struct sockaddr_in net_addr;//创建网络地址结构体
public:
        evutil_socket_t sock;
};
#endif

