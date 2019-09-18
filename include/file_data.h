#ifndef FILE_DATA_H_
#define FILE_DATA_H_

enum class ST{GOOD,BAD};//这里用了C++11中的强枚举类型;限定作用域的枚举类型将名字空间污染降低
class file_data
{
public:
    file_data():fd(-1){}
    int   fd;   
    ST st;
};

#endif
