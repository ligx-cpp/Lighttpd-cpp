# Lighttpd-cpp




         写在前面  感谢CSDN上的一位博主: jiange_zh 他的博客给了我很多启发. 非常感谢！
    
    
                  做这个项目的初衷(姑且称它为项目)：在做这个小项目之前我也是看了不少书，比如:UNIX环境高
     级编程，linux高性能服务器编程，C++primer 曾经也读过经典的C++live555源码 lighttpd源码 linux操作系统
     的简单使用等等还有就不一一列举，我一直秉承着用什么学什么的原则去学习,我知道如果只是去读这些工具书但
     不去实践的话，就会很快忘记曾经读过的内容，所以一直就有个想法去做一个“像样”的项目，一方面这样做会让自
     己很有成就感，其次是可以巩固所学，所以就在网上找一些别人推荐的项目来做一做，很快就确定了两个目标:webbench
     和lighttpd这两个;一个是网站压力测试工具,一个是http轻量级服务器,两个都是C语言写的;所以就萌生了用面向对
     象的思想去实现同样功能的东西;最先动手做的是webbench，因为它简单;我是个半路出家的;和科班出身的人比作东
     西肯定是差了十万八千里;所以就先从简单的做起。废话太多了。。。


                  项目大体流程：lighttpd源码是用的watch-worker(监视者-工作者模式)是由一个进程去fork多个子
     进程的模式,这里我打算用线程池的方式去代替上述模式;优点是可以减少资源开销,提高响应速度；节省了进程多次创
     建和销毁的时间;因为线程池会提前创建线程等待事件的到来。线程池也是从别处学习得来,用到了我的项目当中,然后
     我采用的是libevent来实现我的代码(linux高性能服务器编程中有介绍,源码只是大致的浏览过)项目首先通过在主线
     程监听客户端的连接然后将连接的套接字传到已经阻塞等待的子线程中进行相应的处理。lighttpd的核心处理消息机
     制(状态机)也运用到我的项目当中来:暂时就想到了这么多(后面等项目完成后再补充。。待续)
