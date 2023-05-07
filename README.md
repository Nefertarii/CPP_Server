# C++ Server
一个适配Web页面的小型WebServer  
其核心参照muduo网络库，但是在使用C++17标准的情况下重新制作了一遍    
项目中的命名和操作比较简单易懂，方便阅读和自行修改   

[用于测试的服务器网站](http://106.55.10.242:8000) (原网页域名到期更换成了直接使用ip地址)      
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

## Install  
项目使用的C++17需要更新GCC至少至9.1   
使用 ```sudo apt install GCC``` 安装gcc编译器  
项目采用cmake进行编译，没有使用其他三方库，只需下载编译后即可使用     
使用 ```sudo apt install cmake``` 安装cmake   
在文件夹中新建一个build文件夹 ```mkdir build```  
使用cmake创建makefile文件 ```cmake ..```  
编译文件 ```cmake --build .```  
完成后运行程序 ```./webserver ``` 即可  

## Usage  
若需修改参数可以在顶层CMakeLists.txt 文件中的set(SERVER_SETTING ...)文件中进行设置  
项目内Test文件夹包含了其中类的一些操作示例   
*如使用0～1024端口需要sudo权限运行以绑定端口   
*在使用时可以使用```make MODE=DEBUG```可以对DEBUG日志进行输出且采用Og优化   

## Extra sections  
[EventLoop介绍](https://github.com/Nefertarii/WebServer/blob/master/Include/Eventloop/Eventloop.md)  
[Logger介绍](https://github.com/Nefertarii/WebServer/blob/master/Include/Loger/Logger.md)  
[Net库介绍](https://github.com/Nefertarii/WebServer/blob/master/Include/Net/Net.md)  
[Timer介绍](https://github.com/Nefertarii/WebServer/blob/master/Include/Timer/Timer.md)  
[Web]施工中    

## Background  
在学习完相关知识后想将其整合，于是想到制作一个由多进程+多线程的服务器程序  
在选择方向后，对照muduo一步步进行修改完善，并在未使用其包含的boost三方库情况下用标准库完成了这个项目  
配套有日志库，网络库，一个线程池及一些容器结构   
日志库参考了spdlog和folly的日志库，网页的设计也是对照Github进行的仿写  
项目目前仅做WebServer使用，后续会不断更新    

## Security  
目前还有许多隐藏的问题和不周的写法，如果发现欢迎提交Issues   

## Maintainers  
[@Nefertarii](https://github.com/Nefertarii)  

## Contributing  
这个项目会感谢所有做出帮助的人  
<img src="https://avatars.githubusercontent.com/u/47806751?v=4" height="60" width="60" alt="和我的猫猫">   

## License  
[MIT](https://github.com/Nefertarii/WebServer/blob/master/LICENSE) © Richard Littauer   
