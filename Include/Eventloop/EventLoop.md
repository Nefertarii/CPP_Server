# EventLoop 事件轮询
```TimeQueue```  
定时器功能的实现  
通过二叉搜索树(set)，以(pait<TimeStamp,Timer*>)为key，保证了即使两个计时器(Timer)的到期时间相同，地址也会必定不同  
通过unique_ptr来管理timer，省去了手动管理资源的麻烦  
```Channel```   
封装了对epoll的操作，通过epoll_wait()等待事件发生，发生后通过Channel 类不同的回调函数处理。  
每个Channel负责一个文件描述符(fd)，但不拥有这个描述符，其生命周期由自己的EventLoop(loop_)负责   
Channel必须在析构之前注销自己(EventLoop::RemoveChannel())   
```Poller```   
通过epoll进行事件分发  
由FillActiveChannel() 遍历channles_，找出有活动事件的fd ，填入active_channels   
Poller不拥有channel   
```EventLoop```  
采用了one loop per thread模型，负责事件的分发  
程序的每个IO线程都有一个EventLoop(Reactor)，用于处理读/写事件  
EventLoop为线程的主循环，需要哪个线程运行，通过Channel添加至EventLoop，会自动交给EventLoopThread去执行    
```EventLoopThread```  
程序中真正用于计算的地方  
