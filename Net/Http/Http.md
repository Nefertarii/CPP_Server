nginx处理分11个阶段 能在阶段添加自定义的处理方式  
typedef enum {
    NGX_HTTP_POST_READ_PHASE = 0, 
  
    NGX_HTTP_SERVER_REWRITE_PHASE,  //server块中配置了rewrite指令，重写url
  
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
ngx_http_phases;  
均衡负载 upstream  
正/反向代理  
限流  