//cmd.cpp

#ifndef _CMD_CPP_
#define _CMD_CPP_

#include"../include/utils.h"
#include"cmd.h"
#include<iostream>
#include<sstream>


//根据结束标志获取多行语句并组成一句
std::string CMD::dbget(const char flag,const std::string cli_flag){
    std::string all = "";
    while(1){
        std::string line = "";
        std::getline(std::cin,line);
        // line = " " + SavenUtils::trim(line) + " ";
        int lenth = line.size();
        int ok = 0;
        for(int i=0;i<lenth;i++){
            all += line[i];
            if(line[i] == flag){
                ok = 1;
                break;
            }
        }
        if(ok)break;
        std::cout << cli_flag;
    }
    return all;
}


//字符串存在性查找
inline int stringExists(const std::string line,const std::string aim){
    std::string lowone = SavenUtils::StringtoLower(line);
    return lowone.find(aim) == lowone.npos ? 0 : 1;
}


//特殊函数声明
int not_cmd_check(const std::string order);

//语句粗分类
CMD::orderType CMD::OrderChecker::check(const std::string _cmd){
    printf("[CMD-order]:[%s]\n",_cmd.c_str());
    if(_cmd == "help;"){
        return CMD::orderType::SYS_HELP;
    }else if(_cmd == "quit;"){
        return CMD::orderType::SYS_QUIT;
    }else if(_cmd == "clear;"){
        return CMD::orderType::SYS_CLEAR;
    }
    //特殊检查
    if(not_cmd_check(_cmd)){
        return CMD::orderType::NOT_SQL;
    }
    //可运行的sql语句
    return CMD::orderType::RUN_SQL;
}

//sql粗分类
CMD::orderType CMD::OrderChecker::sql_check(const std::string _cmd){
    //以下语句存在即确定
    if(stringExists(_cmd,"grant")){
        return CMD::orderType::SQL_GRANT;
    }else if(stringExists(_cmd,"revoke")){
        return CMD::orderType::SQL_REVOKE;
    }else if(stringExists(_cmd,"create")){
        return CMD::orderType::SQL_CREATE;
    }else if(stringExists(_cmd,"drop")){
        return CMD::orderType::SQL_DROP;
    }
    //以下分类只根据第一个单词确定
    std::stringstream ss(_cmd);
    std::string one = "";
    ss >> one;
    if(one == "select"){
        return CMD::orderType::SQL_SELECT;
    }else if(one == "insert"){
        return CMD::orderType::SQL_INSERT;
    }else if(one == "update"){
        return CMD::orderType::SQL_UPDATE;
    }else if(one == "delete"){
        return CMD::orderType::SQL_DELETE;
    }
    return CMD::orderType::SQL_SPECIAL;
}

/* int not_cmd_check(const std::string order); */
/* 需要在这里实现对特殊命令的封禁 */

int not_cmd_check(const std::string order){
    //如果是不可执行的sql则返回1
    return 0;
}

#endif //_CMD_CPP_

