#ifndef FILE_DATA_H_
#define FILE_DATA_H_
#include<string>
typedef enum
{
    INIT,
    READ,
    DONE,
    NOT_EXIST,
    NOT_ACCESS
} static_state_t;
class file_data
{
public:
    file_data(): s_fd(-1) {}

    int          s_fd;
    char s_buf[10*1024];
    static_state_t s_state;
};

#endif
