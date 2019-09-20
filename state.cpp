#include"state.h"
void init_state::handle(msg_thread* me){
             //执行操作
             for(size_t i=0;i<me->plugin_set.size();++i){
                     plugin* plugin_m=static_cast<plugin*>(me->plugin_set[i]);          
                     plugin_m->init_plugin(me,i);
		}
             m_sponse->change_state(new ready_write_state(m_sponse));
             delete this;
}

void ready_write_state::handle(msg_thread* me){
             //执行操作
             for(size_t i=0;i<me->plugin_set.size();++i){
                     plugin* plugin_m=static_cast<plugin*>(me->plugin_set[i]);
                     plugin_m->ResponseStart(me,i);
                }
             m_sponse->change_state(new write_state(m_sponse));
             delete this;
}

void write_state::handle(msg_thread* me){
             //执行操作
             for(size_t i=0;i<me->plugin_set.size();++i){
                     plugin* plugin_m=static_cast<plugin*>(me->plugin_set[i]);
                     plugin_m->Write(me,i);//获得响应体

                }
             me->outbuf.reserve(10*1024);
             me->outbuf=me->sponse_msg.make_response();//先写好响应头            
             me->outbuf+=(me->sponse_msg.body);//写响应体
             m_sponse->change_state(new end_state(m_sponse));
             delete this;
}

void end_state::handle(msg_thread* me){
             //执行操作
             for(size_t i=0;i<me->plugin_set.size();++i){
                     plugin* plugin_m=static_cast<plugin*>(me->plugin_set[i]);
                     plugin_m->ResponseEnd(me,i);
                }
             me->outbuf.clear();//将字符串清空
             //sponse->change_state(new end_state());
             delete this;
        }
