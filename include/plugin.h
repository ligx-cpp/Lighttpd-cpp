#ifndef CONFIG_H_
#define CONFIG_H_
#include<stdio.h>
#include<vector>
#include<string>
#include"msg_thread.h"
//系统运行时在需要某个功能的时候动态加载的模块叫做插件
//我为什么要选择用插件架构？首先插件架构方便功能的扩展;我的插件仅仅实现了是做了写不同的html这部分因为在我看来读消息这部分是通用的并没有说是会以不同的形式去读.所以我打算在动态库中只实现写消息,因为写消息可能是千变万化的这与html页面相对应我可能想显示主页或者如果客户端没有找到相应的资源我这时想返回404消息等等;然后是可以降低模块之间的依赖
class plugin{//这是一个接口类在核心程序中实现,我还需要一个插件管理器需要用单例模式来实现,负责插件的加载，注册，和卸载;在C++ 中一般都使用懒汉式单例,但懒汉式单例可能会有线程安全,异常安全问题
public:
	plugin();
	virtual ~plugin();//析沟函数必须是虚函数
	virtual int init_plugin(msg_thread *me,int i);
	virtual int ResponseStart(msg_thread *me,int i);
	virtual int Write(msg_thread *me,int i);
	virtual int ResponseEnd(msg_thread *me,int i);
        virtual void Close(msg_thread *me,int i);
	std::vector<std::string> path_list;//插件链表

        //typedef plugin* (*SetupPlugin)();typedef的功能是定义新的类型。第一句就是定义了一种SetupPlugin的类型，并定义这种类型为指向某种函数的指针，这种函数不传入参数并返回plugin*类型。后面就可以像使用int,char一样使用SetupPlugin了。第二行的代码便使用这个新类型定义的变量setup_plugin;
        typedef plugin* (*SetupPlugin)();//这里借用了网上的加载类的解法
        SetupPlugin setup_plugin;     

        typedef void (*Destroyplugin)(plugin*);
        Destroyplugin remove_plugin;
  
        int   plugin_i;
        void *handle;
};


#endif
