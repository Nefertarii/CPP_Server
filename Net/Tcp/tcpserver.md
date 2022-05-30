Acceptor中包括有Socket Channel等  
Socket是一个RAIIhandle，封装了socket描述符的生命周期  
Acceptor中的Socket是linstening socket,Channel用于观察此socket上的可读事件，并回调Handleread(),Socket则会调用accept()来接受新连接，并回调用户指定的callback  
Tcpconnection为网络库中的核心类，通过shared_ptr来管理，且继承enable_shared_form_this  
Tcpconnection使用Channel来获得socket上的IO事件，它会自己处理可写事件，而将可读事件通过messagecallback传达给使用者  
Tcpconnection拥有TCP socket，析构函数会close(fd)(由Socket调用)，该类表示的是一次TCP连接，一旦断开，该对象将没什么用处了，且没有主动发起连接的功能，其参数是已经建立好连接的socket fd  
Tcpconnection在socket可写时，Channel会调用Handle_write()，将发送数据，一旦发送完毕会立即停止观察可写事件，避免busy loop  
Tcpconnection还设置了TCP No Delay/TCP keepalive和SIGPIPE的处理方式  
epoll返回的是活动的fd列表，方便使用且更高效  