//这里用的是加载类的方法来实现的
//本服务器设计的动态链接库的职责就是只负责写过程

#ifndef STATIC_HTML_H_
#define STATIC_HTML_H_
#include "plugin.h"
#include "msg_thread.h"
#include <unistd.h>
#include <regex.h>//因为C是最快的
//下面三个是open函数的头文件
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include"file_data.h"

class static_html : public plugin{
virtual int init_plugin(msg_thread *me,int i){             
               file_data *data = new file_data();
               me->plugin_file_data.push_back(data);
               return 0;
}

virtual int ResponseStart(msg_thread *me,int i){
               file_data  *data = static_cast<file_data*>(me->plugin_file_data[i]);
               std::string temp="/html/index.html";
               //接下来用正则表达时匹配的方法找到响应的html网页
               /*regex_t    reg;//regex_t 是一个结构体数据类型，用来存放编译后的规则表达式
               regmatch_t pmatch[1];//regmatch_t 是一个结构体数据类型,成员rm_so存放匹配文本串在目标串中的开始位置,rm_eo 存放结束位置。

              if()
        int cnt=regcomp(&reg,"^/html/[^/]*$", REG_EXTENDED);//这个函数把指定的规则表达式pattern编译成一种特定的数据格式compiled，这样可以使匹配更有效。函数regexec会使用这个数据在目标文本串中进行模式匹配。执行成功返回０。
        if(cnt!=0){
          std::cout<<"regcomp操作失败！"<<std::endl;//执行成功才返回0
        }
        int ret = regexec(&reg, temp.c_str(), 1, pmatch, 0);//当我们编译好规则表达式后，就可以用regexec匹配我们的目标文本串了，如果在编译规则表达式的时候没有指定cflags的参数为REG_NEWLINE，则默认情况下是忽略换行符的，也就是把整个文本串当作一个字符串处理。执行成功返回０。“1”可能指的是我只会去得到一个匹配字串
*/

        //这次匹配如果能匹配成功那么就执行读写操作,如果匹配不成功则一律是404notfound;

                if (temp==me->parsered_msg.url)
                {//这是匹配成功
                         data->st=ST::GOOD;
                }
                else
                {//这是匹配不成功 
                         data->st=ST::BAD;
                }  
                return 0;
      }


virtual int Write(msg_thread *me,int i){

                file_data  *data = static_cast<file_data*>(me->plugin_file_data[i]);
                std::string temp=me->parsered_msg.url;
                if (data->st==ST::GOOD)
                {
                         data->fd= open(temp.substr(1).c_str(), O_RDONLY); //substr()函数只有一个数字1表示从下标为1开始一直到结尾
                }else
                {//此时一律打开的是404notfound
                         me->sponse_msg.code  = 404;
                         me->sponse_msg.code_des = "File don't exist";
                         data->fd= open("html/404.html", O_RDONLY);//所以这里直接给了绝对路径
                }
                char buf[10*1024];
                int ret = read(data->fd, buf, sizeof(buf));//从html文件中读取消息;s_fd是html文件的文件描述符   
                me->sponse_msg.body = buf;
                memset(buf, 0, sizeof(buf));//这里应该立刻将字符串置为空否则会出现上次内容还留在字符数组中的现象
                return 0;
      }

virtual int ResponseEnd(msg_thread *me,int i){
                file_data *data = static_cast<file_data*>(me->plugin_file_data[i]);
                close(data->fd);
                data->fd = -1;
                delete data;      
                return 0;
     }

};
#endif

extern "C" plugin* SetupPlugin(){

                return new static_html();
       
}

extern "C" plugin* DestroyPlugin(plugin* plugin_t){

                delete plugin_t;       

}
