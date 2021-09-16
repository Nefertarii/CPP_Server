# httpweb

### README仍需更新 目前在完善程序和规划流程2021-09-16

### 更新日志/新计划/Change log
[用于测试的服务器网站](http://159.75.51.91:8000/)       
(仍在完善中 打算更改前端页面的显示内容，主站考虑做成主要功能为查询信息的，附有登录注册功能的网站2021-9-16)  


### 项目背景/Background
在学习完相关知识后想将其整合，于是想到制作一个有交互功能，且考虑使用数据库存储相关数据的web服务器  
从基础做起，打算制作一个使用自制的数据库并有多进程多线程的服务器程序  
由此加深对基础知识的理解和对大型项目制作过程的体验  
( 一开始考虑搭配unreal制作一款游戏并作为游戏服务器使用，无奈过程中无法抽空学习unreal的使用而放弃  
  但目前结构也在向能使用不同协议方便扩展的运行方式而开发 )  
整体由管理进程控制数据库进程和服务器进程的启动以及启动时的设置  
也可单独使用数据库和服务器  

#### 整体构思
由可在运行中使用命令进行更改设置的多进程程序为主体目标,进行开发

其中,打算自行实现的额外目标：  
- [ ] 打算从http升级成https处理 
- [ ] 内存的分配(可以自动回收垃圾的垃圾回收器)  
- [ ] 字符串的处理(char* 需要一直转换，实现一个贴合程序的string类)   
- [ ] 线程池的实现(目前正在实现的难点08-25)  

整体功能/模块大致如下：  
* 管理程序 (可接受键盘输入调整设置,并能控制所有的会话子进程)
	* 根据输入更改设置
	* 控制其他进程的开启/关闭 
	* 控制其他进程的运行设置
	* 管理进程的资源使用 
* 服务器进程 (用于对请求进行处理的主要进程，受管理程序控制，通过多线程处理会话)   
	* 建立连接: 根据tcp协议建立http连接----------------------(主线程)
	* 接受请求: 在持续连接的基础上处理对端发送的报文-----(主线程)
	* 处理请求: 处理对端发送的报文并验证相关信息----------(子线程)
	* 本地资源处理: 根据处理的请求判断所需资源的合法性---(主线程)
		* 需要访问数据库时：  
		* 数据库的连接
		* 数据库的请求传递
		* 数据库的返回处理
	* 响应构建: 构建http响应和相应json-----------------------(子线程)
	* 返回响应: 交由子线程处理，返回构建的响应------------(子线程)
	* 日志处理: 存储从启动到终止时所有步骤的记录----------(主线程)
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

### 项目负责人/Maintainers
[@Nefertarii](https://github.com/Nefertarii)  

### License
[MIT](https://github.com/Nefertarii/WebServer/blob/master/LICENSE) © Richard Littauer   
