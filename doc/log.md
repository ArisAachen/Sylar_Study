## 说明
此文档用于说明设计log模块

## 模块

Log Manager: 用于管理所有 Logger 类别, 使用 Aris_LOG_INFO(message)    
Logger: 用于管理不同类型的日志， 如：控制台类型的输入 std::cout  文件类型的日志 syslog 或者网络日志 
LogAppender: 管理某一类的日志, 不同类型的日志包含不同的 Level