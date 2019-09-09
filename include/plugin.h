#ifndef CONFIG_H_
#define CONFIG_H_
#include<stdio.h>
#include<list>
#include<string>
class plugin{
public:
	plugin();
	~plugin();
	int init_plugin();
	virtual int ResponseStart();
	virtual int Write();
	virtual int ResponseEnd();
	std::list<std::string> path_list;//插件链表
        std::string CGIRoot;		/*CGI根路径*/
	std::string DefaultFile;	/*默认文件名称*/
	std::string DocumentRoot;       /*根文件路径*/
	std::string ConfigFile;		/*配置文件路径和名称*/
       
        SetupPlugin setup_plugin;
        DestroyPlugin remove_plugin;
        int   plugin_i;
};
typedef Plugin* (*SetupPlugin)();//这里借用了网上的加载类的解法     
typedef void (*Destroy_plugin)(plugin*);

#endif
