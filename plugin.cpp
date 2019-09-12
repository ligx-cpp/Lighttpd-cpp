#include"plugin.h"

plugin::plugin(){
	DefaultFile="index.html";
	DocumentRoot ="./html";
	ConfigFile="./slighttpd.conf";
	path_list.push_back("plugin/static_html/static_html.so");
}


int plugin::init_plugin(msg_thread *me,int i){

}
int plugin::ResponseStart(msg_thread *me,int i){

}
int plugin::Write(msg_thread *me,int i){

}
int plugin::ResponseEnd(msg_thread *me,int i){

}
void plugin::Close(msg_thread *me,int i)
{

}
plugin::~plugin(){
}
