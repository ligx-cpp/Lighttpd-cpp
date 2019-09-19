#ifndef HANDLE_STATE_H_
#define HANDLE_STATE_H_
#include"state.h"
class Response{
public:
        Response(){m_state = new init_state(this);}

        void request(){
               if(NULL != m_state)
               {
                    m_state->handle();
               }
        }

        void change_state(State* state){
               m_state = state;
        }
private:
        State *m_state;
};
#endif
