#ifndef HANDLE_STATE_H_
#define HANDLE_STATE_H_
#include"state_base.h"
#include"msg_thread.h"
class Response{
public:
        Response();

        void request(msg_thread* me);

        void change_state(State* state);
private:
        State *m_state;
};
#endif
