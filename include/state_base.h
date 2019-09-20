#ifndef STATE_BASE_H_
#define STATE_BASE_H_
#include <msg_thread.h>
class State{
public:
        virtual void handle(msg_thread* me)=0;
        virtual ~State(){}
};
#endif
