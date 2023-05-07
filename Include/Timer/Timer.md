# TimeStamp.h:  
``` class TimeStamp ```   
记录时间戳，使用一个长整型来记录自纪元以来的毫秒数。  
``` TimeStamp() ```   
是默认构造函数，创建一个未定义的时间戳。   
``` TimeStamp(long microseconds) ```   
是构造函数，创建一个时间戳，其值为传入的微秒数转换成毫秒后的值。  
``` static TimeStamp TimeStampAdd(TimeStamp timestamp, double seconds) ```    
将传入的时间戳加上指定的秒数（返回为TimeStamp对象）。    
``` static double TimeStampDiff(TimeStamp high, TimeStamp low) ```    
计算两个时间戳之间的时间差（以秒为单位）。  
``` void TimeAdd(double seconds) ```   
将时间戳本身加上指定的秒数。     
``` double TimeDiff(TimeStamp other) ```   
计算该时间戳与另一个时间戳之间的时间差（返回以秒为单位）。  
``` bool operator<(TimeStamp rhs) ```  
比较两个时间戳的大小  
``` bool operator==(TimeStamp rhs) ```   
比较两个时间戳是否相等。  
``` long GetTime() ```   
返回该时间戳的值（以毫秒为单位）。  
``` void Swap(TimeStamp& other) ```   
交换该时间戳与另一个时间戳的值。  
``` bool Valid() const ```  
检查该时间戳是否有效。  
``` TimeStamp Invalid() ```   
静态方法，返回一个无效的时间戳对象。  

``` const int microseconds_per_second ```       
每秒的微秒数  
``` const int microseconds_per_milliseconds ```   
每毫秒的微秒数  
``` const int millisecond_per_second ```        
每秒的毫秒数  
# clock.h
``` class Clock ```  
进行时间处理的一个类  
``` using Timer_Us = std::chrono::microseconds; ```  
微秒  
``` using Timer_Ms = std::chrono::milliseconds; ```  
毫秒  
``` using Timer_Sec = std::chrono::seconds; ```  
秒  
``` using Timer_Clock = std::chrono::high_resolution_clock; ```  
高精度时钟  
``` template <typename Rep, typename Period> ```  
``` using Timer_Duration = std::chrono::duration<Rep, Period>; ```    
模板类型，用于表示一个时间段。   
``` template <typename Type> ```  
``` using Timer_Point = std::chrono::time_point<Timer_Clock, Type>; ```  
模板类型，用于表示一个时刻。  
``` Clock() ```  
构造函数，创建一个 Clock 对象，并记录创建时的时间戳（毫秒）   
``` long RuntimeUs() ```  
返回从创建 Clock 对象到当前时刻的时间（微秒）   
``` long RuntimeMs() ```  
返回从创建 Clock 对象到当前时刻的时间（毫秒）   
``` long RuntimeSec() ```  
返回从创建 Clock 对象到当前时刻的时间（秒）   
``` static long NowtimeUs() ```  
返回当前时刻的时间戳（微秒）   
``` static long NowtimeMs() ```    
返回当前时刻的时间戳（毫秒）    
``` static long NowtimeSec() ```   
返回当前时刻的时间戳（秒）   
``` static std::string ToString(TimeStamp time)  ```  
将一个时间戳转换为字符串表示，使用 ToSecString 方法来转换   
``` static std::string ToStringSec(long timestamp_sec, std::string format = "%b %m %Y %H:%M:%S") ```  
将一个以秒为单位的时间戳转换为字符串表示  
``` static std::string ToStringMs(long timestamp_ms, std::string format = "%b %m %Y %H:%M:%S") ```  
将一个以毫秒为单位的时间戳转换为字符串表示    
``` static std::string ToStringUs(long timestamp_us, std::string format = "%b %m %Y %H:%M:%S")  ```  
将一个以微秒为单位的时间戳转换为字符串表示    
``` ~Clock() ```  
析构函数，不做任何操作。   
# timer.h
``` class Timer ```  
一个定时器  
``` const std::function<void()> callback_ ```   
回调函数对象，当定时器到期时需要执行的函数  
``` TimeStamp expiration_ ```   
定时器的到期时间点，用于定时器执行回调函数的时机  
``` const double interval_ ```   
定时器的执行间隔，如果是一次性定时器，则该值为 0  
``` const bool repeat_ ```   
定时器是否是可重复执行的，如果是，则每次执行回调函数后，需要更新到期时间（当前时间加上间隔）   
``` const int sequence_ ```   
定时器对象的序号，用于区分不同的定时器对象  
``` static std::atomic<int> create_num_ ```   
已经创建的定时器对象的数量，用于分配序号  
``` Timer(const std::function<void()>& callback, TimeStamp when, double interval) ```   
构造函数，使用需要传入回调函数、到期时间点和执行间隔  
``` void Run() const ```   
执行回调函数  
``` void Restart(TimeStamp now) ```   
重启定时器，传入当前时间点  
``` bool Repeat() ```   
返回定时器是否是可重复执行的  
``` int Sequence() ```   
返回该定时器的序号  
``` static int CreateNum() ```   
返回已经创建的所有定时器的数量  
``` TimeStamp Expiration() ```   
返回定时器到期的时间点  