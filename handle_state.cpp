#include"handle_state.h"
#include"state.h"
Response::Response(){
           m_state = new init_state(this);
}

void Response::change_state(State* state){
           m_state = state;
}

void Response::request(msg_thread* me){
           if(NULL != m_state)
           {
                 m_state->handle(me);
           }
}

