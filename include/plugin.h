#ifndef CONFIG_H_
#define CONFIG_H_
#include<stdio.h>
#include<vector>
#include<string>
#include"msg_thread.h"
class plugin{
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
