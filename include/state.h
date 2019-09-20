#ifndef STATE_H_
#define STATE_H_
//允许一个对象在其内部状态改变时改变它的行为，使对象看起来似乎修改了它的类。 
#include<iostream>
#include <msg_thread.h>
#include"state_base.h"
#include "plugin.h"
#include"handle_state.h"
//C++11 中使用关键字 override，可以避免在派生类中忘记重写虚函数的错误
//C++11 中的 override 关键字，可以显式的在派生类中声明，哪些成员函数需要被重写，如果没被重写，则编译器会报错。
//如果派生类里面是像重写虚函数 就加上关键字override 这样编译器可以辅助检查是不是正确重写，如果没加这个关键字 也没什么严重的error 只是少了编译器检查的安全性
class Response;


class init_state :public State{//overide
public:
        init_state(Response *sponse):m_sponse(sponse) {}
        virtual void handle(msg_thread* me) override;
private:
        Response *m_sponse;
};

class ready_write_state :public State{//overide
public:
        ready_write_state(Response *sponse):m_sponse(sponse) {}
        virtual void handle(msg_thread* me) override;
private:
        Response *m_sponse;
};

class write_state :public State{//overide
public:
        write_state(Response *sponse):m_sponse(sponse) {}
        virtual void handle(msg_thread* me) override;
private:
        Response *m_sponse;
};

class end_state :public State{//overide
public:
        end_state(Response *sponse):m_sponse(sponse) {}
        virtual void handle(msg_thread* me) override;
private:
        Response *m_sponse;
};

#endif
