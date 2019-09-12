#子类函数要依赖于父类
INCLUDE=-I../../include#因为头文件都在这里面

plugin_static.so:static_html.o ../../plugin.o
	g++  $^ -shared -o $@

static_html.o:static_html.cpp
	g++ -fPIC -c $< $(INCLUDE) -o $@

../../plugin.o:../../plugin.cpp 
	g++ -fPIC -c $< $(INCLUDE) -o $@

clean:
	rm -rf ../../plugin.o *.o #把生成的plugin.o也删掉
#这里两个参数-shared和-fPIC. -shared,是共享库.-fPIC使.so文件的代码段变为真正意义上的共享.如果不加-fPIC,则加载.so文件的代码段时,代码段引用的数据对象需要重定位, 重定位会修改代码段的内容,这就造成每个使用这个.so文件代码段的进程在内核里都会生成这个.so文件代码段的copy.每个copy都不一样,取决于 这个.so文件代码段和数据段内存映射的位置.不加fPIC编译出来的so,是要再加载时根据加载到的位置再次重定位的
