# Lighttpd-cpp


        感谢CSDN上的一位博主：jiange_zh 他的博客给了我很多启发.写在前面 非常感谢！
2019-09-21日晚于电航楼：
项目总结：这个总结我是写了又删,删了又写.总是觉得写的不好~~哈哈 可能是我现在太兴奋了。
  先说一下我为什么不去进一步的让这个项目变得更完美(添加日志还有修改细节)。主要是因为-时间,之所以有机
会做这个东西是因为在暑假我有大量的时间去考虑问题,去查资料,去学习.但在暑假之前由于我灵光乍现(踩了狗屎)在我
的研究课题上有了个想法,并且效果还挺好(我的研究课题是图像去噪。。。),跟老师沟通之后老师说这个想法可以
发个论文出去,顺便写个专利;现在开学了,老师催着我去忙论文的事情,除了论文还要我去写专利.所以事情慢慢
就多了起来,也就没有那么多纯粹的时间和心思去做这个项目了.所以更进一步的优化工作就只能搁置。

  我的感想:我是很满意我的这个小项目,我之前也讲过这是我第一次做这方面的东西,我的本意是把我看过的计
算机的书和学到的一些知识都运用起来;不至于让知识都停留在书本的层面上,里面有些东西确实是我强加上去
的,最明显的就是一些设计模式的运用.我曾经看过一个视频,那个老师说设计模式最初引进来是为了应付大
项目(十几万行)才去运用设计模式去使代码看起来更具有条理性,而我在仅仅1000多行(不算我调用的第三方
库)左右的代码中我就用了好几个设计模式。。。比如 单例模式(懒汉) 状态模式;当然还有一些在无形中就
用上的比如 观察者模式 工厂方法模式等等吧.设计模式看起来更像是一种书写代码的方式而不是特定的一
项技术;至少我目前的理解是这样.

  今后要做什么?之后我的重点可能就是去领扣上刷刷题,读一下剑指offer,把
曾经看过的书再拿出来翻翻看看.之前看过的书里面的细节很长一段时间不看估计会忘记;会更注重理论基础知识
的学习,就先这样吧.如果有机会我会继续更新的~~~hh

2019-09-21更新：这这次更新算是接近这个项目的尾声,我目前希望完成的功能都已经完成,要说我还有什么
想加入进去的，那就是日志功能;日志系统对于我们调试多线程程序是非常有必要的,它可以看出程序运行到哪
一步出了问题,方便我们对症下药.我的初步想法是通过引入单例模式来实现这个日志;为什么我要用单例模式
呢？因为我们可能在多线程同时运行时的任何一个线程的任何时候都可能调用日志模块,去把输出消息写到日
志文件当中去,我们则可能需要一个全局函数;在任何地方都可以取用；然而利用全局变量是违背封装性的,不
可取。所以一个合适的做法是利用单例模式;单例模式可以保证一个类只有一个实例，并且可以提供一个访问
它的全局访问点。还有一点是这个日志系统的线程安全问题,当多个线程同时向同一个日志文件中写数据时肯
定会出现问题;这也是我为什么把这一项功能放在最后的一个重要原因；这项功能的工作量应该是不小。说到
线程安全这方面的知识要是扯起来给估计就是很大篇幅了所以对于自己实现日志还是先说到这。。。我也看过
网上的一些实现基本上都是输出时间和分等级的错误.比如debug,警告,错误等等...Linux下也有实现好的日志
系统安装成功就能用,比如log4xx系列,这个完全可以直接拿来用;鉴于本项目只是作为学习.我本人还是更倾向
于自己去实现;就没有把这个日志系统直接拿来用.本意是留待以后有时间了去好好研究一下.关于项目就说到这里

2019-09-17更新：这次更新完成了整个项目的基本功能.即完成了从浏览器的搜索窗输入192.168.56.101:4000/html/index.html
然后从浏览器中显示index.html中的内容.这次更新基本上是接近了项目的尾声,按照我的想法接下来要做的是
对这个项目增加一些特色功能比如状态机功能;这个暂且放置;先说一下在这期间遇到的一些问题:首先是调用第
三方库解析http请求时始终不成功;我首先是找到了http_parser(第三方库的名字)的github源码地址;并且仔细
阅读了它的使用说明并且我在网上找到了一个使用示例.并且也是照着做的;不过还是有问题它始终显示我解析的
url的前四个字符(因为首先解析的就是url)然后就显示terminate called after throwing an instance of 'std::logic_error' 
what(): basic_string::_S_construct NULL not valid;网上的解析是对空指针进行操作
然后左改右改也没找到问题出在哪里...大概是在那天晚上我被折磨的筋疲力尽的时候我开始静下心来去仔细
分析他的使用说明中的每一个参数的意义的时候我发现了我的错误来源:在http_parser_execute(parser,&settings,buffer,buffer_num)
这一句中,问题就处在最后一个参数上,我初以为最后一个参数像read()函数那样是buffer中的字节大小,然后我的写法是
http_parser_execute(parser,&settings,buffer,sizeof(buffer));但是问题在于我传进来的buffer是一个指针,
sizeof(buffer)此时仅仅表示该指针所占内存的大小,而不是buffer所指向内存的大小,这样继续执行下去就报错了;
改了以后就能正确运行了并显示了.问题最后给出第三方库的github源码地址https://github.com/nodejs/http-parser.git 
在此处附上一份静态库和动态库之间的区别：静态库和动态库的最大区别,(1)静态情况下,把库直接加载到程序中,而
动态库链接的时候,它只是保留接口,将动态库与程序代码独立,这样就可以提高代码的可复用度，和降低程序的耦合
度。(2)静态库在程序编译时会被连接到目标代码中，程序运行时将不再需要该静态库。动态库在程序编译时并不
会被连接到目标代码中，而是在程序运行是才被载入，因此在程序运行时还需要动态库存在
        
2019-09-12更新：这次更新给我的程序在回显的基础上添加了一些功能;只有回显显得整个程序很单调这次更新
我添加了能使我的服务器在浏览器上链接的这项功能其实这个功能;刚开始没有什么想法;故在网上查找了一些
资料;也参考了 jiange_zh 大佬的博客,最终也总结出来了要实现这个功能我需要怎么做...

其实也很简单;我之前也做过简单的服务器只不过是之前输入URL请求都是在命令行;现在我把输入URL的地方放
在了浏览器的搜索框里链接原理还是一样的;同理当我的服务器收到消息以后要返回给客户端;简单情况是我的
客户端也是命令行;命令行收到消息显示在屏幕上;而现在我返回的消息要在浏览器中显示;这在返回的响应体
中需要一些前段代码,在此处不是我们的重点,暂且不论.我们需要关注的是怎么把在命令行输入和响应的消息
转移到浏览器上?我们在此引入了插件系统.正如lighttpd那本书上所说的那样:灵活的插件结构使用户可以轻
易地修改服务器功能而不用改变lighttpd的核心代码.多个插件的同时存在使lighttpd应用程序以一种合理的
方式组织和调用.我想说明的是加载类这个方法是一个难点同时也是我的这个项目的亮点之一.我们知道如果
你想在实现插件时用动态库来实现,如果这个库用C语言来实现那么是很轻松的.用dlopen,dlsym,dlclose,来实
现就可以了,但是如果你想用C++来实现动态的装载一个库的话(由于dlopen这类函数是C语言的函数并没有提供
一个合适的方式来装载类;还有一个原因是C++的name mangling问题)还是非常困难的;解决方案就是使用extern "C"
这可以算是C++的关键字;加载类的时候因为我们无法通过new来创建类的实例,因为类不是在可执行文件中定义的,
是在动态库.so中的;那么如何来解决呢？答案是利用多态性.我们在可执行中文件定义一个带有虚成员函数的接
口基类,而在动态库中定义派生类.接下来是利用设计模式中的工厂方法模式在动态库中定义两个附加的函数一个
用来创建一个类的实例，一个用来delete该类对象;且这两个函数都用extern "C"来修饰.还有就是在使用加载
类时需要注意的点这里我就不一一介绍了有兴趣的话可以去http://blog.chinaunix.net/uid-12072359-id-2960897.html 这里看看,作者讲解的非常详细我也是在这里得到了如何去解决这个问题。

我的解决方案就是来自这里用的是加载类的方法;目前还存在的问题是对配置文件不是很清楚,除此之外我还想
在之后的时间里在里面加入日志系统;目前lighttpd的核心状态机模型还没有加入进去;待续。。。
　　


2019-09-06更新：解决了一个线程只能运行连接一个客户端的问题;这个问题也反映了我很愚蠢;既然我用的是
libevent库;libevent底层封装的都是多路I/O函数,例如select,epool这样的函数;这些都是能同时响应多个连
接的函数;怎么会只能连接一个客户端呢？所以这确实是我的代码逻辑有问题;很快我发现了问题是我的线程池
模型和libevent库不太兼容，我的线程池的消息传递方式是通过设置条件变量的方式;我原先的想法是每当我
的主线程监听到来自客户端的连接之后返回一个套接字;之后这个套接字就会被存放在一个消息队列里面当我
主动随机唤醒一个线程(为什么是随即的?因为条件变量的机制就是随机唤醒一个线程)后这个线程就会从阻塞
中退出然后将消息从消息队列中取出;进入base循环执行读写功能;事实上这确实是我有点想当然；而且随机唤
醒一个线程这种机制对于高并发连接时有可能会导致某一个连接的连接数很多;而其他线程的连接数较少,从而
导致负载不均衡问题;然后是上一次更新曾经出现的问题当某一客户端异常关闭时会导致服务器端异常关闭;
而且在已经关闭掉的客户端之后新连接的客户端无法与服务器进行正常读写(连接是正常连接的);我在综合考
量了这些问题之后我把这些问题定位在两个点上;一:条件变量机制问题很大,不可用.二:读写机制有问题,手
动写的读写(read和write)回调函数可能在异常处理方面考虑的不是那么全面从而导致了一系列问题。然后在
网上搜索有关的解决方案;我竟然找到了和我有同样想法的博客。。。我运行了他的程序，他解决了我上面说
的一部分问题；但是没有解决客户端异常关闭后;后面继续连接上来的客户端读写异常问题;然后借鉴了他的做
法;将条件变量机制改为用管道在线程间通信并且用专门的一个(event)事件去监听管道读端,一旦有消息就调用
读写函数;完美的和libevent模型相契合;这里给大佬点个赞！.还有一个问题没解决“读写问题”因为刚开始看的
启蒙博客是jiange_zh博主只是用了一下没有缓冲区的,在我走投无路的抱着试试看的想法我学习了一下带缓冲
区的event事实上常用的网络编程都是用的bufferevent;为什么我会去尝试bufferevent呢?因为我发现bufferevent
中的读写函数是被封装的既然是别人封装好的;我觉得他的异常读取机制肯定比我这个菜鸟做的更好,考虑的更
多;事实上确实如此;bufferevent确实完美的解决了这个问题完成了我的这个项目的前一些部分;回显部分;后续待更新。。。


2019-09-02更新：本程序遇到的一些问题:首先是在我开启多个客户端连接时如果其中一个客户端强制关闭后;相
应的服务器端也会直接退出这个问题 解释如下：当强行关闭客户终端后，client进程交付给初始进程。等初始
进程查询到client后将它杀掉。但是在杀掉之前，由于关掉了终端（主要是关掉了输入缓冲区),导致本来阻
塞中的cin或scanf返回EOF，程序得以继续执行send和recv操作。服务器发现client有消息传入,但是在尝试回应
client的时候client被初始进程杀掉。然后就变成了给已经关闭的socket发送数据。当服务器监听并接受一
个客户端链接的时候, 可以不断向客户端发送数据, 这时如果客户端断开socket链接, 服务器继续向一个关
闭的socket 发送数据(send, write)的时候, 系统会默认对服务器进程发送一个SIGPIPE信号, 这个信号的
默认动作就是终止当前服务器进程。  解决方案:我利用read函数的读取错误返回来的信息,一旦返回的是错
误信息，我就让rwhand对象中的“监听事件循环”退出并且析构掉rwhand对象这样程序就不会出现异常退出问题

 另一个大问题是如何让我的一个线程同时处理多个连接;现在的情况是我一个线程只能处理一个连接如果当
前连接不放开的话就无法收到另一个新套接字的读写消息;其他问题遇到了再补充。。。


   2019-08更新：做这个项目的初衷(姑且称它为项目)：我一直秉承着用什么学什么的原则去学习,我知道
如果只是去读这些工具书但不去实践的话，就会很快忘记曾经读过的内容，所以一直就有个想法去做一个
“像样”的项目，一方面这样做会让自己很有成就感，其次是可以巩固所学，所以就在网上找一些别人推荐
的项目来做一做，很快就确定了两个目标:webbench和lighttpd这两个；一个是网站压力测试工具,一个是http
轻量级服务器,两个都是C语言写的;所以就萌生了用面向对象的思想去实现同样功能的东西;最先动手做的是
webbench，因为它简单;我是个半路出家的;和科班出身的人比作东西肯定是差了十万八千里;所以就先从简单
的做起。废话太多了。。。


    
   项目大体流程：lighttpd源码是用的watch-worker(监视者-工作者模式)是由一个进程去fork多个子进程
的模式,这里我打算用线程池的方式去代替上述模式;优点是可以减少资源开销,提高响应速度；节省了进程多次
创建和销毁的时间;因为线程池会提前创建线程等待事件的到来。线程池也是从别处学习得来,用到了我的项目当
中,然后我采用的是libevent来实现我的代码(linux高性能服务器编程中有介绍,源码只是大致的浏览过)项目首
先通过在主线程监听客户端的连接然后将连接的套接字传到已经阻塞等待的子线程中进行相应的处理。lighttpd
的核心处理消息机制(状态机)也运用到我的项目当中来:暂时就想到了这么多(后面等项目完成后再补充。。待续)
