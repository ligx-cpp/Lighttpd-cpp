#include"config.h"

plugin::plugin(){
	CGIROOT ="./cgi";
	DefaultFile="index.html";
	DocumentRoot ="./htdocs";
	ConfigFile="./slighttpd.conf";
	path_list.push_back("plugin/static_html.so");
        //path_list.push_back("plugin/plugin_cgi.so");
}

int plugin::init_plugin(){
	return 0;
}
plugin::~plugin(){
}
