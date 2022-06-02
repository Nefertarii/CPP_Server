# C++ Server
一个适配Web页面的小型网络库  
参照muduo网络库，在使用C++17标准库的条件下重新制作了一遍    

[用于测试的服务器网站](http://webwasi.com/)      
[更新日志](https://github.com/Nefertarii/WebServer/blob/master/ChangeLog.md)  
[![license](https://img.shields.io/badge/license-MIT-blue)](LICENSE)

## 目录
- [说明](#description)
- [背景](#background)
- [安装](#install)
- [用法](#usage)
- [额外部分](#extra-sections)
- [安全](#security)
- [维护者](#maintainers)
- [贡献](#contributing)
- [许可证](#license)

## Description  
因为参考的muduo，整体基于Reactor模式实现  
负责事件循环的部分由Eventloop实现，监听对应的事件包括有Socket可读写事件、定时器事件等  
- EventLoop  
采用的模型是one loop per thread + threadpool，因此每个线程至多有一个EventLoop   
EventLoop通过Channel和Poller配合使用，使用epoll的通知机制，以在事件可用时调用相应的回调函数   
EventLoop的定时器实现由TimerQueue、Timer、TimerId三个类实现   
EventLoopThread  
因为IO线程不一定是主线程，通过IO线程池来管理，对应的类是EventLoopThreadPool   
- Tcp   
TcpServer通过管理accept获得的TcpConnection可直接使用   
TcpClient类似与TcpServer但只管理一个TcpConnection   
而Tcpconnection是网络操作的封装，不对外可见，通过上级接口使用  
- Threadpool  
线程池通过自定义的TaskQueue来调用要求的任务，且空闲的线程可以通过窃取其他线程池的任务来保证效率   
- Log  
Logger通过一个vector\<Sink\>来记录保存日志，且能分发至不同的Sink方便使用   
Logging中有一个静态logger用于默认的日志写入，方便使用  
 
## Install  
项目使用的C++17需要更新GCC至少至9.1   
```apt install GCC```   
项目采用Makefile进行编译，没有使用其他三方库，只需下载编译后即可使用     

## Usage  
在主文件夹下使用```make```进行编译   
使用```./Wasi_Server```即可运行  
若需修改参数可以在Net文件夹下的setting.conf文件中进行设置  
项目内还包含有Test文件夹包含了其中类的一些操作示例，在Test文件夹下使用```make```即可   
如果需要包含至其他项目，可以考虑包含头文件并使用build/lib 目录下的libCPPServer.a静态库  
*如使用默认设置需要sudo权限以绑定端口   
*在使用make时可以使用```make MODE=DEBUG```可以对DEBUG日志进行输出且采用Og优化  

## Extra sections  
[EventLoop详细](https://github.com/Nefertarii/WebServer/blob/master/Base/Poll/eventloop.md)  
[TcpServer详细](https://github.com/Nefertarii/WebServer/blob/master/Net/Tcp/tcpserver.md)  

## Background  
用于学习的一个简易C++ TCP服务器  
在学习完相关知识后想将其整合，于是想到制作一个由多进程+多线程的服务器程序  
在选择后，对照muduo一步步进行修改完善,在未使用其包含的boost三方库情况下用标准库完成了这个项目  
配套有日志库，网络库，一个线程池及一些容器结构 
后续也会不断更新  

## Security  
目前并未进行完整的项目测试，如果发现问题欢迎提交问题   

## Maintainers  
[@Nefertarii](https://github.com/Nefertarii)  

## Contributing  
这个项目会感谢所有做出帮助的人  
<img src="https://avatars.githubusercontent.com/u/47806751?v=4" height="60" width="60" alt="和我的猫猫">   

## License  
[MIT](https://github.com/Nefertarii/WebServer/blob/master/LICENSE) © Richard Littauer   