void init_state::handle(msg_thread* me){
             //执行操作
             for(size_t i=0;i<me->plugin_set.size();++i){
                     plugin* plugin_m=static_cast<plugin*>(me->plugin_set[i]);          
                     plugin_m->init_plugin(me,i);
		}
std::cout<<"11111111111111111111111111"<<std::endl;
             m_sponse->change_state(new ready_write_state(m_sponse));
             delete this;
}

void ready_write_state::handle(msg_thread* me){
             //执行操作
             for(size_t i=0;i<me->plugin_set.size();++i){
                     plugin* plugin_m=static_cast<plugin*>(me->plugin_set[i]);
                     plugin_m->ResponseStart(me,i);
                }
std::cout<<"222222222222222222222222222"<<std::endl;
             m_sponse->change_state(new write_state(m_sponse));
             delete this;
}

void write_state::handle(msg_thread* me){
             //执行操作
             for(size_t i=0;i<me->plugin_set.size();++i){
                     plugin* plugin_m=static_cast<plugin*>(me->plugin_set[i]);
                     plugin_m->Write(me,i);//获得响应体

                }
 std::cout<<"333333333333333333333333"<<std::endl;
             m_sponse->change_state(new end_state(m_sponse));
             delete this;
}

void end_state::handle(msg_thread* me){
             //执行操作
             for(size_t i=0;i<me->plugin_set.size();++i){
                     plugin* plugin_m=static_cast<plugin*>(me->plugin_set[i]);
                     plugin_m->ResponseEnd(me,i);
                }
std::cout<<"444444444444444444444444"<<std::endl;
             //sponse->change_state(new end_state());
             delete this;
        }
