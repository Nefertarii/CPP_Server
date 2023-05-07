# Net 网络接收/传输
```SocketPack```    
封装了系统socket的一些操作  
```Socket```  
通过获得的socket_fd，封装了一些函数方便操作  
```Inetaddress```  
表示网络地址，将IP地址和端口号封装成一个了类，方便使用，还提供了一些常用的网络地址操作函数。  
```Acceptor```   
用于监听指定的地址和端口，接收(accept()) 新的连接请求  
内部class ，供TcpSever使用，生命周期也由TcpServer控制   
```Connector```  
用于连接远程地址   
```TcpConnection```  
负责管理连接的状态，处理读写事件，处理连接相关的各种事件：注册事件，何时接收数据，何时关闭连接等等。  
该类是网络库中最核心的类，通过Channel 获取socket 上的事件，自动处理writeable 事件，将readable 事件通过MessageCallBack 传输给客户。
该类不可再生，为一次性连接，并且没有发起连接的能力，传入的是已经建立好的sockfd，所以起始状态为kConnecting。同时不会主动关闭连接，只会在对方关闭，本地read() 返回0 后触发关闭。  
```TcpClient```   
提供建立和管理TCP 连接的方法。 抽象出许多使用套接字的低级细节，提供一个高级接口以通过 TCP 进行通信。  
创建 TcpClient 对象时，它通过连接到其 IP 地址和端口号来建立到远程服务器的连接。 建立连接后，TcpClient 对象提供通过连接发送和接收数据的方法。  
```TcpServer```   
与TcpClient相同，包装了一些低级接口以供使用。  
TcpServer 侦听指定IP 地址和端口号上的连接，创建一个新的TcpConnection 对象来连接请求。  
新连接到达时，Acceptor 会回调