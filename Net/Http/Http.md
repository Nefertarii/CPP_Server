nginx处理分11个阶段 能在阶段添加自定义的处理方式  
typedef enum {
    NGX_HTTP_POST_READ_PHASE = 0, // 接收完请求头之后的第一个阶段，它位于uri重写之前，实际上很少有模块会注册在该阶段，默认的情况下，该阶段被跳过
    NGX_HTTP_SERVER_REWRITE_PHASE, //server块中配置了rewrite指令，重写url
  
    NGX_HTTP_FIND_CONFIG_PHASE,   //查找匹配的location配置；不能自定义handler；
    NGX_HTTP_REWRITE_PHASE,       //location块中配置了rewrite指令，重写url
    NGX_HTTP_POST_REWRITE_PHASE,  //检查是否发生了url重写，如果有，重新回到FIND_CONFIG阶段；不能自定义handler；
  
    NGX_HTTP_PREACCESS_PHASE,     //访问控制，比如限流模块会注册handler到此阶段
  
    NGX_HTTP_ACCESS_PHASE,        //访问权限控制，比如基于ip黑白名单的权限控制，基于用户名密码的权限控制等
    NGX_HTTP_POST_ACCESS_PHASE,   //根据访问权限控制阶段做相应处理；不能自定义handler；
  
    NGX_HTTP_TRY_FILES_PHASE,     //只有配置了try_files指令，才会有此阶段；不能自定义handler；
    NGX_HTTP_CONTENT_PHASE,       //内容产生阶段，返回响应给客户端
  
    NGX_HTTP_LOG_PHASE            //日志记录
}   

GET /home.html HTTP/1.1
Host: developer.mozilla.org
User-Agent: Mozilla/5.0 (Macintosh; Intel Mac OS X 10.9; rv:50.0) Gecko/20100101 Firefox/50.0
Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8
Accept-Language: en-US,en;q=0.5
Accept-Encoding: gzip, deflate, br
Referer: https://developer.mozilla.org/testpage.html
Connection: keep-alive
Upgrade-Insecure-Requests: 1
If-Modified-Since: Mon, 18 Jul 2016 02:36:04 GMT
If-None-Match: "c561c68d0ba92bbeb8b0fff2a9199f722e3a621a"
Cache-Control: max-age=0

Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/avif,image/webp,image/apng,*/*;q=0.8,application/signed-exchange;v=b3;q=0.9
Accept-Encoding: gzip, deflate, br
Accept-Language: zh-CN,zh;q=0.9
Connection: keep-alive
Cookie: BAIDUID
DNT: 1
Host: www.baidu.com
sec-ch-ua: " Not A;Brand";v="99", "Chromium";v="99", "Google Chrome";v="99"
sec-ch-ua-mobile: ?0
sec-ch-ua-platform: "Windows"
Sec-Fetch-Dest: document
Sec-Fetch-Mode: navigate
Sec-Fetch-Site: none
Sec-Fetch-User: ?1
Upgrade-Insecure-Requests: 1
User-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/99.0.4844.74 Safari/537.36


HTTP/1.1 200 OK
Connection: Keep-Alive
Content-Encoding: gzip
Content-Type: text/html; charset=utf-8
Date: Thu, 11 Aug 2016 15:23:13 GMT
Keep-Alive: timeout=5, max=1000
Last-Modified: Mon, 25 Jul 2016 04:32:39 GMT



specifier	Replaced by	                                                            Example
%a	        Abbreviated weekday name *	                                            Thu
%A	        Full weekday name *	                                                    Thursday
%b	        Abbreviated month name *	                                            Aug
%B	        Full month name *	                                                    August
%c	        Date and time representation *	                                        Thu Aug 23 14:55:02 2001
%C	        Year divided by 100 and truncated to integer (00-99)	                20
%d	        Day of the month, zero-padded (01-31)	                                23
%D	        Short MM/DD/YY date, equivalent to %m/%d/%y	                            08/23/01
%e	        Day of the month, space-padded ( 1-31)	                                23
%F	        Short YYYY-MM-DD date, equivalent to %Y-%m-%d	                        2001-08-23
%g	        Week-based year, last two digits (00-99)	                            01
%G	        Week-based year	                                                        2001
%h	        Abbreviated month name * (same as %b)	                                Aug
%H	        Hour in 24h format (00-23)	                                            14
%I	        Hour in 12h format (01-12)	                                            02
%j	        Day of the year (001-366)	                                            235
%m	        Month as a decimal number   (01-12)	                                    08
%M	        Minute (00-59)	                                                        55
%n	        New-line character ('\n')	                
%p	        AM or PM designation	                                                PM
%r	        12-hour clock time *	                                                02:55:02 pm
%R	        24-hour HH:MM time, equivalent to %H:%M	                                14:55
%S	        Second (00-61)	                                                        02
%t	        Horizontal-tab character ('\t')	
%T	        ISO 8601 time format (HH:MM:SS), equivalent to %H:%M:%S	                14:55:02
%u	        ISO 8601 weekday as number with Monday as 1 (1-7)	                    4
%U	        Week number with the first Sunday as the first day of week one (00-53)	33
%V	        ISO 8601 week number (01-53)	                                        34
%w	        Weekday as a decimal number with Sunday as 0 (0-6)	                    4
%W	        Week number with the first Monday as the first day of week one (00-53)	34
%x	        Date representation *	                                                08/23/01
%X	        Time representation *	                                                14:55:02
%y	        Year, last two digits (00-99)	                                        01
%Y	        Year	                                                                2001
%z	        ISO 8601 offset from UTC in timezone (1 minute=1, 1 hour=100)
            If timezone cannot be determined, no characters	                        +100
%Z	        Timezone name or abbreviation *
            If timezone cannot be determined, no characters	                        CDT
%%	        A % sign	                                                            %