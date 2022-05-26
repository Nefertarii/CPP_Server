# C++ Server (Readme待修改）
### 更新日志/Change log
[用于测试的服务器网站](http://webwasi.com/)      
[日志](https://github.com/Nefertarii/WebServer/blob/master/ChangeLog.md)

### 项目背景/Background
在学习完相关知识后想将其整合，于是想到制作一个有交互功能的服务器  
打算制作一个由多进程+多线程的服务器程序  
对照muduo一步步进行修改完善,由此加深对基础知识的理解和大型项目制作过程的体验   
配套有日志库，网络库，一个线程池及一些容器结构
(一开始考虑搭配unreal搭配用作专属服务器使用，目前也在向能使用不同协议方便扩展的运行方式而开发 )   
作为我的第一个大型规划，后续会不断更新  

### 整体构思 
#### 网络库(仿muduo,有删减且仅使用标准库文件)   
muudo是Reactor模式的现代C++网络库,采用非阻塞IO模型,基于事件驱动和回调  
#### 日志库(参考了Folly库和spdlog的设计)   
#### 线程池   


### 项目的安装及使用/Install & Usage
新版服务器的使用(还在开发中...)  

旧版服务器的使用(真的很烂,等新写的吧)   
项目编译使用了 Makefile  
在文件夹HTTPserver/build中为HTTP WEB服务器  
使用命令 ```make MODE=DEBUG``` 可启动调试模式，该模式下将直接输出各个处理的日志信息，且使用-Og编译  
直接使用命令 ```make``` 将会储存日志信息到文件中，并使用-O2编译  
启动时会根据目录下的Http.conf来设置相关参数  
若不绑定0\~1024端口，使用 ```./HTTPserver [conf file]``` 即可启动服务器进程  
绑定1\~1024端口需要添加权限，需使用 ```sudo ./HTTPserver [conf file]``` 才可成功启动  
如 ```./HTTPserver /home/ubuntu/Http.conf``` 启动HTTP WEB服务器。  

### 项目负责/Maintainers
[@Nefertarii](https://github.com/Nefertarii)  

### License
[MIT](https://github.com/Nefertarii/WebServer/blob/master/LICENSE) © Richard Littauer   
