plugin_static.so:static_html.o ../plugin.o
	g++ -o $@ static_html.o ../plugin.o -shared -pthread -levent

static_html.o:static_html.cpp ../include/plugin.h ../lib/http-parser/http_parser.h
	g++ -fPIC -o $@ -c $< -I../include -I../lib/http-parser

../plugin.o:../plugin.cpp ../include/plugin.h
	g++ -fPIC -o $@ -c $< -I../include

clean:
	rm -rf ../../src/plugin.o *.o *.so
