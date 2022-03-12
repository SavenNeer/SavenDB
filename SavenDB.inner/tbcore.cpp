// tbcore.cpp

#ifndef _TBCORE_CPP_
#define _TBCORE_CPP_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tbcore.h"

/* 全局缓冲变量 */

//全局公共缓冲区域
std::vector<std::vector<std::string> > public_res;
//公共缓冲区使用标记
int public_cnt = 0;
//公共错误结果输出缓存
std::string errMsg;


/* 编译可选子模块 */
#ifndef _SUB_OPERATOR_
#define _SUB_OPERATOR_

#include <sqlite3.h>
#include <iostream>
#include <string>

//回调函数
static int callback(void *data, int argc, char **argv, char **azColName){
    std::vector<std::vector<std::string> >& rhs = public_res;
    std::vector<std::string> oneline;

    //首部属性列存在性检测
    if(public_cnt == 0){
        std::vector<std::string> header;
        for(int i=0;i<argc;i++){
            std::string attr = std::string(azColName[i]);
            header.push_back(attr);
        }
        rhs.push_back(header);
        public_cnt = 1;
    }

    //元组插入
    for(int i=0; i<argc; i++){
        // 生成vector<vector>嵌套结果
        std::string one = (argv[i] == NULL) ? "NULL" : std::string(argv[i]);
        oneline.push_back(one);
    }
    rhs.push_back(oneline);
    return 0;
}

//一次数据库操作
int inner_checker(const std::string table_name,const std::string order){

    //使用到的变量
    sqlite3 *db;
    char *zErrMsg = 0;
    int rc;
    char sql[3000];
    const char* data = "Callback function called";

    //初始化数据库名称
    char tbname[100];
    memset(tbname,0,sizeof(tbname));
    sprintf(tbname,"%s",table_name.c_str());

    //打开数据库
    rc = 0;
    rc = sqlite3_open(tbname, &db);
    if( rc ){
        // printf("Can't open database: %s\n", sqlite3_errmsg(db));
        errMsg = "[DBcore]-Cannot Open DB: " + std::string(sqlite3_errmsg(db));
        std::cout << "[ori-error]:" << errMsg << std::endl;
        return -1;
    }else{
        printf("Opened database successfully\n");
        errMsg = "";
    }

    //拷贝数据库sql命令
    memset(sql,0,sizeof(sql));
    sprintf(sql,"%s",order.c_str());
    
    //运行sql语句
    rc = sqlite3_exec(db, sql, callback, (void*)data, &zErrMsg);
    if( rc != SQLITE_OK ){
        //输出错误信息
        // printf("SQL Error: %s\n", zErrMsg);
        errMsg = std::string(zErrMsg);//输出错误结果到全局变量
        std::cout << "[ori-error]:" << errMsg << std::endl;
        sqlite3_free(zErrMsg);
        zErrMsg = NULL;
    }else{
        printf("Operation done successfully\n");
        errMsg = "";
    }
    
    //关闭数据库
    sqlite3_close(db);
    return 0;
}

#endif //_SUB_OPERATOR_


/* 核心类函数实现 */

TBCore::SavenDB_TBCore::SavenDB_TBCore(){
    this->init();
}

void TBCore::SavenDB_TBCore::init(){
    this->res.clear();
    this->hasVaildres = false;
    errMsg = "";
}

void TBCore::SavenDB_TBCore::flush(){
    this->init();
}

// @input 表名称 表命令
// @return 需要输出的结果的数据条数(包括属性说明行)
// 需要说明的是 如果没有需要输出的结果 那么返回结果为0
int TBCore::SavenDB_TBCore::check(const std::string table_name,const std::string order){
    //清空结果的公共缓冲区域
    public_res.clear();
    public_cnt = 0;
    //内部变量初始化
    this->flush();
    //调用内核控制函数
    int ret = inner_checker(table_name,order);
    // printf("[DBcore]-check:ret = %d\n",ret);
    //结果获取
    this->errMsg = errMsg;
    this->res = public_res;
    //获取结果条数
    return res.size(); 
}

#endif //_TBCORE_CPP_


