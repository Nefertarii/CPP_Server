# Logger 日志记录
该模块可以将日志存放至本体、网络，或打印在控制台上，甚至是其他地方。  
通过追加器(Appender)管理日志槽(Sink)的灵活性，一条日志能根据需求同时存放至不同的位置  
```header.h```   
存放了常用的类   
```LogLevel```  
日志等级的一个分类   
```LoggerSetting```  
用于追加器中进行日志转换的设置  
```LogMsg(日志行)```  
每条需要记录的日志都会先转换成该类，再做处理  
```Sink```  
日插槽，将已经格式化过的日志行通过插槽输出到插槽中指定位置  
通过LogSink纯虚函数可以自己编写需要的插槽
```LogFormatter```  
用于将其他地方记录的信息转换成统一格式的日志行的类  
```Appender```  
追加器，作为统一管理日志槽(Sink)的类，一个追加器可以拥有多个日志槽  
负责将日志从最基础的形式，转换成一个日志行类，再转至指定的格式的标准  
同时有着指定日志等级过滤功能，可以将设定的日志等级直接丢弃  
```Logger```   
日志模块的完整类，包装了一个静态追加器方便使用  