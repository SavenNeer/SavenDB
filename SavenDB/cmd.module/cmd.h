//cmd.h

#ifndef _CMD_H_
#define _CMD_H_


#include<string>

namespace CMD{

    //获取以分号结尾的语句-可多行
    //参数:标识提示符
    std::string dbget(const char flag,const std::string cli_flag);

    enum orderType{
        RUN_SQL,//允许运行的sql
        NOT_SQL,//不可运行的sql

        SYS_HELP,//帮助类命令
        SYS_QUIT,//退出命令
        SYS_CLEAR,//清屏幕命令

        //sql分类
        SQL_GRANT,
        SQL_REVOKE,
        //-表操作
        SQL_CREATE,
        SQL_DROP,
        //-元组操作
        SQL_INSERT,
        SQL_SELECT,
        SQL_UPDATE,
        SQL_DELETE,
        //特殊-用于系统级操作
        SQL_SPECIAL
    };

    //命令类型分类器
    class OrderChecker{
    public:
        //命令大致分类
        CMD::orderType check(const std::string _cmd);
        //sql粗分类
        CMD::orderType sql_check(const std::string _cmd);
    };

};





#endif //_CMD_H_


