# httpweb
### 更新计划/Plan
[用于测试的服务器网站](http://159.75.51.91:8000/)       
仍在完善中 打算更改前端页面的显示内容，考虑做成主要功能为各类信息收集整理，带有登录注册功能的网站(2021-9-24)  
数据库的开发所需实现的细节和要求比计划时的高出太多，暂缓开发(2021-10-04)  
整体的开发大致如此了，整体框架已经确定，剩余的功能、页面和需要添加的操作将在以后再进行补充(2021-10-10)  
前端页面制作完成后考虑另建储存库(2021-10-17)

### 项目背景/Background
在学习完相关知识后想将其整合，于是想到制作一个有交互功能，且考虑使用数据库存储相关数据的web服务器  
打算制作一个使用自制的数据库，并由多进程管理+多线程的服务器程序    
由此加深对基础知识的理解和大型项目制作过程的体验  
( 一开始考虑搭配unreal制作一款游戏并作为游戏服务器使用，要配套开发时间周期太长  
  目前结构也在向能使用不同协议方便扩展的运行方式而开发 )   
程序整体由管理进程控制数据库进程和服务器进程的启动以及启动时的设置  
也可单独使用数据库和服务器
作为我的第一个大型规划，后续会不断更新  

#### 整体构思
其中,项目的难点
- [ ] 打算从http升级成https处理(整体完成后考虑)
- [ ] 内存的分配(可以自动回收垃圾的垃圾回收器,预计和string类一起开发)  
- [ ] 字符串的处理(char* 需要一直转换，实现一个贴合程序的string类)   
- [ ] 线程池的实现(目前正在实现的难点(合适的线程池对整体的语法，特性，安全性较难完善 09-17)  
- [ ] 多进程管理(需要单独设计一个程序控制 目前还没有大致的想法 10-17)  
- [ ] 数据库的制作(需要解决数据的问题，包括数据的储存和加载，还有不同模块和整体的整合是最需要考虑的部分,计划最后开发 10-17)  

整体功能/模块大致如下：  
* 管理程序 (可接受输入调整设置,并能控制所有的会话子进程)
	* 根据输入更改相关设置
	* 控制其他进程的开启/关闭
	* 管理进程的资源使用(考虑实现) 
* 服务器进程 (用于对请求进行处理的主要进程，受管理程序控制，通过多线程处理会话)   
	* 连接管理: 在启动后初始化设置及连接控制
	* 建立连接: 根据tcp协议建立http连接
	* 接受请求: 在持续连接的基础上处理对端发送的报文
	* 处理请求: 处理对端发送的报文并验证相关信息
	* 本地资源处理: 根据处理的请求判断所需资源的合法性
	* 对数据库的连接控制
		* 数据库的连接
		* 数据库的请求发送
		* 数据库的返回接收
	* 响应构建: 构建http响应和相应json
	* 返回响应: 交由子线程处理，返回构建的响应
	* 日志处理: 存储从启动到终止时所有步骤的记录
* 数据库进程 (储存方式使用图数据结构)
	* [详细参见Database.md](https://github.com/Nefertarii/WebServer/blob/master/Database/DataBase.md)

管理进程中的可用命令 (待完成...)  

### 项目的安装及使用/Install & Usage
项目编译使用了 Makefile      
在文件夹HTTPserver/build中为HTTP WEB服务器    
使用命令 ```make MODE=DEBUG``` 可启动调试模式，该模式下将直接输出各个处理的日志信息，且使用-Og编译     
直接使用命令 ```make``` 将会储存日志信息到文件中，并使用-O2编译    
启动时会根据目录下的Http.conf来设置相关参数   
若不绑定0\~1024端口，使用 ```./HTTPserver [conf file]``` 即可启动服务器进程      
绑定1\~1024端口需要添加权限，需使用 ```sudo ./HTTPserver [conf file]``` 才可成功启动     
如 ```./HTTPserver /home/ubuntu/Http.conf``` 启动HTTP WEB服务器。     

### 更新日志/Change log
#### Version1.2(2021-10-06) 
大幅修改了原程序的代码结构，同时完善了对POST请求的处理  
对访问的错误处理和类结构进行了优化  
(该栏-更新日志 以后将记录每次所添加/删除的功能及结构变化)
#### Version1.2.1(2021-10-18)
改进了HTML代码结构 缩小了代码量
增加了其他页面用于测试服务器

### 项目负责人/Maintainers
[@Nefertarii](https://github.com/Nefertarii)  

### License
[MIT](https://github.com/Nefertarii/WebServer/blob/master/LICENSE) © Richard Littauer   
