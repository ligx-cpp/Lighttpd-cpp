//这里用的是加载类的方法来实现的
//本服务器设计的动态链接库的职责就是只负责写过程

#ifndef STATIC_HTML_H_
#define STATIC_HTML_H_
#include "plugin.h"
#include "msg_thread.h"
#include <unistd.h>
#include <regex.h>
//下面三个是open函数的头文件
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
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
               me->plugin_data_slots[i] = data;
               return true;
      }

      virtual int ResponseStart(msg_thread *me,int i){
               static_data_t  *data = static_cast<static_data_t*>(me->plugin_data_slots[i]);
               http_quest_msg *request = me->parsered_msg;//把解析好的消息放在一个临时对象里
               //接下来用正则表达时匹配的方法找到响应的html网页
               regex_t    reg;//regex_t 是一个结构体数据类型，用来存放编译后的规则表达式
               regmatch_t pmatch;//regmatch_t 是一个结构体数据类型,成员rm_so存放匹配文本串在目标串中的开始位置,rm_eo 存放结束位置。
               int        ret;

        regcomp(&reg, "^/html/*$", REG_EXTENDED);//这个函数把指定的规则表达式pattern编译成一种特定的数据格式compiled，这样可以使匹配更有效。函数regexec会使用这个数据在目标文本串中进行模式匹配。执行成功返回０。
        ret = regexec(&reg, request->http_url.c_str(), 1, &pmatch, 0);//当我们编译好规则表达式后，就可以用regexec匹配我们的目标文本串了，如果在编译规则表达式的时候没有指定cflags的参数为REG_NEWLINE，则默认情况下是忽略换行符的，也就是把整个文本串当作一个字符串处理。执行成功返回０。“1”可能指的是我只会去得到一个匹配字串
        
        if (ret)
        {
            data->s_state = NOT_ACCESS;
        }
        else
        {
            std::string path = request->http_url.substr(1);

            if (access(path.c_str(), R_OK) == -1)//access函数检查调用进程是否可以对指定的文件执行某种操作 F_OK 值为0，判断文件是否存在 X_OK 值为1，判断对文件是可执行权限 W_OK 值为2，判断对文件是否有写权限 R_OK 值为4，判断对文件是否有读权限
            {
                data->s_state = NOT_EXIST;
            }
            else
            {
                data->s_state = INIT;//正常来说应该是执行到这一步
            }
        }
        
        return true;
      }


      virtual int Write(msg_thread *me,int i)
      {
            static_data_t  *data = static_cast<static_data_t*>(me->plugin_data_slots[i]);
            http_quest_msg *request = me->parsered_msg;

            if (data->s_state == INIT)
            {
                 data->s_state = READ;
                 data->s_fd= open(request->http_url.substr(1).c_str(), O_RDONLY); //substr()函数只有一个数字1表示从下标为1开始一直到结尾
            }
            /*else if (data->s_state == NOT_ACCESS)
            {
                 me->sponse_msg.http_code    = 404;
                 me->sponse_msg.http_phrase 	= "Access Deny";
                 return PLUGIN_READY;
            }
            else if (data->s_state == NOT_EXIST)
            {
                 me->sponse_msg.http_code    = 403;
                 me->sponse_msg.http_phrase 	= "File don't exist";
                 return PLUGIN_READY;
            }*/

        int ret = read(data->s_fd, &data->s_buf[0], data->s_buf.capacity());//从html文件中读取消息;s_fd是html文件的文件描述符
        me->sponse_msg->http_body += data->s_buf[0];
        return 0;
        /*if (ret <= 0)
        {
            data->s_state = DONE;
            me->sponse_msg.http_body += data->s_data;
            return true;
        }
        else
        {
            data->s_data.append(&data->s_buf[0], 0, ret);
            return true;
        }*/
    }

    virtual int ResponseEnd(msg_thread *me,int i)
    {
        static_data_t *data = static_cast<static_data_t*>(me->plugin_data_slots[i]);

        if (data->s_state == DONE)
        {
            close(data->s_fd);
            data->s_fd = -1;
            data->s_data.clear();
        }
        
        return 0;
    }

    virtual void Close(msg_thread *me,int i)
    {
        static_data_t *data = static_cast<static_data_t*>(me->plugin_data_slots[i]);

        if (data->s_fd != -1)
        {
            close(data->s_fd);
        }

        delete data;
    }
};
#endif

extern "C" plugin* SetupPlugin(){//有时你想在运行时加载一个库（并使用其中的函数），这在你为你的程序写一些插件或模块架构的时候经常发生。在C语言中，加载一个库轻而易举（调用dlopen、dlsym和dlclose就够了），但对C++来说，情况稍微复杂。动态加载一个C++库的困难一部分是因为C++的name mangling（译者注：也有人把它翻译为“名字毁坏”，我觉得还是不翻译好），另一部分是因为dlopen API是用C语言实现的，因而没有提供一个合适的方式来装载类。一般来说，dlopen和dlsym是来处理C库中的函数的，对于C++中存在的name mangle问题，类的问题就不易处理：解决方案extern "C"。用 extern "C"声明的函数将使用函数名作符号名，就像C函数一样。因此，只有非成员函数才能被声明为extern "C";
         return new static_html();
       
}

extern "C" plugin* DestroyPlugin(plugin* plugin_t){
         delete plugin_t;       
}
