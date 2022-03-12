// tbcore.h
#pragma once
#ifndef _TBCORE_H_
#define _TBCORE_H_


#include <string>
#include <vector>


namespace TBCore{

    class SavenDB_TBCore{
    public:
        //结果内容
        std::vector<std::vector<std::string> > res;
        //结果是否指向了有效的内容
        bool hasVaildres;
        //错误信息
        std::string errMsg;
    public:
        //构造函数
        SavenDB_TBCore();
        //成员初始化 功能相同
        void init();
        void flush();
    public:
        //表内语句执行 启动前自动调用flush()
        int check(const std::string table_name,const std::string order);
    };
};


#endif //_TBCORE_H_

