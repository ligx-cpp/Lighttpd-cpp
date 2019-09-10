extern "C" plugin* SetupPlugin(){//有时你想在运行时加载一个库（并使用其中的函数），这在你为你的程序写一些插件或模块架构的时候经常发生。在C语言中，加载一个库轻而易举（调用dlopen、dlsym和dlclose就够了），但对C++来说，情况稍微复杂。动态加载一个C++库的困难一部分是因为C++的name mangling（译者注：也有人把它翻译为“名字毁坏”，我觉得还是不翻译好），另一部分是因为dlopen API是用C语言实现的，因而没有提供一个合适的方式来装载类。一般来说，dlopen和dlsym是来处理C库中的函数的，对于C++中存在的name mangle问题，类的问题就不易处理：解决方案extern "C"。用 extern "C"声明的函数将使用函数名作符号名，就像C函数一样。因此，只有非成员函数才能被声明为extern "C";
         return new static_html();
       
}

extern "C" plugin* DestroyPlugin(plugin* plugin_t){
         delete plugin_t;       
}

//这里用的是加载类的方法来实现的
//本服务器设计的动态链接库的职责就是只负责写过程

#ifndef STATIC_HTML_H_
#define STATIC_HTML_H_
#include "plugin.h"
#include "msg_thread.h"
typedef enum
{
    INIT,
    READ,
    DONE,
    NOT_EXIST,
    NOT_ACCESS
} static_state_t;

typedef struct StaticData
{
    StaticData(): s_fd(-1), s_state(INIT) {}

    int          s_fd;
    std::string  s_buf;
    std::string  s_data;
    static_state_t s_state;
} static_data_t;

class static_html : public plugin{
      virtual int init_plugin(msg_thread *me,int i){
                     static_data_t *data = new static_data_t();
                     data->s_buf.reserve(10 * 1024);  
                     me->plugin_data_slots[index] = data;
                     return true;
      }
      virtual int ResponseStart(){
               
      }
};
#endif

