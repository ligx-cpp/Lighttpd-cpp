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
        static netaddr* m_stance;
	static netaddr* getInstance();  
	int net_init(string ip,int port);
	~netaddr();
private:
        netaddr(){}
        netaddr(const netaddr& other){}//默认和拷贝构造函数私有化
        netaddr& operator = (const netaddr&){};//禁止赋值
	struct sockaddr_in name_addr;//创建网络地址结构体
        class CGarbo   //它的唯一工作就是在析构函数中删除netaddr的实例
	{
	public:
		~CGarbo()
		{
			if(netaddr::m_stance)
				delete netaddr::m_stance;
		}
	};
	static CGarbo Garbo;  //定义一个静态成员变量，程序结束时，系统会自动调用它的析构函数

};

//程序在结束的时候，系统会自动析构所有的全局变量。事实上，系统也会析构所有的类的静态成员变量，就像这些静态成员也是全局变量一样。利用这个特征，我们可以在单例类中定义一个这样的静态成员变量，而它的唯一工作就是在析构函数中删除单例类的实例
#endif

