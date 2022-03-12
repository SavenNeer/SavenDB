//dbuser.cpp

#ifndef _DBUSER_CPP_
#define _DBUSER_CPP_


#include"dbuser.h"
#include<fstream>
#include<sstream>


//所有权限集合
std::vector<std::string> allrt = {
    "select",
    "update",
    "delete",
    "insert",
    "create",
    "drop",
    "grant",
    "revoke",
    "_vld"
};


//构造函数
DBUser::UserInfo::UserInfo(){
    this->rights.clear();
    this->hasRight.clear();
    this->user = this->password = "";
}

//设置密码
void DBUser::UserInfo::setPassword(const std::string password){
    this->password = password;
}

//检查用户权限存在性
int DBUser::UserInfo::hasRightonTable(const std::string tbname,const std::string right){
    if(this->hasRight[tbname] == 1){
        if(this->rights[tbname][right] == 1){
            return 1;
        }
    }
    return 0;
}

//添加权限
void DBUser::UserInfo::addRight(const std::string tbname,std::vector<std::string> rhs){
    int lenth = rhs.size();//要求添加的权限列表
    this->hasRight[tbname] = 1;//确保激活数据库权限
    std::map<std::string,int>& rt = this->rights[tbname];
    //逐个添加权限
    for(int i=0;i<lenth;i++) { rt[rhs[i]] = 1; }
}

//权限删除
void DBUser::UserInfo::delRight(const std::string tbname,std::vector<std::string> rhs){
    if(this->hasRight[tbname] == 1){
        int lenth = rhs.size();
        for(int i=0;i<lenth;i++){
            this->rights[tbname][rhs[i]] = 0;
        }
        //检查是否在该表上还拥有权限
        lenth = allrt.size();
        int cnt = 0;
        for(int i=0;i<lenth;i++){
            if(this->rights[tbname][allrt[i]] == 1){
                cnt++;
            }
        }
        if(cnt == 0) this->hasRight[tbname] = 0;
    }
}

//用户授权
int DBUser::UserInfo::grantRight(UserInfo& user,const std::string tbname,const std::vector<std::string> rhs){
    //检查所要求的授权属性是否存在
    int lenth = rhs.size();
    for(int i=0;i<lenth;i++){
        if(this->hasRightonTable(tbname,rhs[i]) == 0){
            printf("[error]:You don't have/grant-right %s\n",rhs[i].c_str());
            return -1;
        }
    }
    //授权
    user.addRight(tbname,rhs);
    return 0;
}


//用户收权
int DBUser::UserInfo::revokeRight(UserInfo& user,const std::string tbname,const std::vector<std::string> rhs){
    //检查所要求收回的权限自己是否存在
    int lenth = rhs.size();
    for(int i=0;i<lenth;i++){
        if(this->hasRightonTable(tbname,rhs[i]) == 0){
            printf("[error]:You don't have/revoke-right %s\n",rhs[i].c_str());
            return -1;
        }
    }
    //收权
    user.delRight(tbname,rhs);
}

//生成向系统文件.right写入的账户信息字符串
std::string DBUser::UserInfo::generateRight2String(){
    std::string all = this->user + " _sys_user _vld" + "\n";
    //遍历所有支持的表
    std::map<std::string,int>::iterator iter = this->hasRight.begin();
    while(iter != this->hasRight.end()){
        if(iter->second == 1){
            //支持的表名称
            std::string tbname = iter->first;
            if(tbname != "_sys_user"){
                std::string line = this->user + " " + tbname;
                //查询表名上所有有用的权限
                int lenth = allrt.size();
                for(int i=0;i<lenth;i++){
                    if(this->rights[tbname][allrt[i]] == 1){
                        line = line + " " + allrt[i];
                    }
                }
                all = all + line + "\n";
            }
        }
        iter++;
    }
    // printf("[right]\n%s\n",all.c_str());
    return all;
}

void DBUser::UserInfo::show(){
    printf("show():\n%s\n",this->generateRight2String().c_str());
}


/* 数据库账户与权限管理 */

DBUser::UserChecker::UserChecker(){
    this->ReadFile();
}

DBUser::UserChecker::~UserChecker(){
    this->WriteFile();
}

//数据读取
void DBUser::UserChecker::ReadFile(){
    //清空现有的所有用户数据信息
    this->_mapper.clear();
    //读取文件
    std::string line = "";
    std::fstream file;
    file.open(this->right_path,std::ios::in);
    while(getline(file,line)){
        int lenth = line.size();
        if(lenth == 0) continue;
        //注释行
        if(line[0] == '#') continue;
        //解析语句
        std::string username;
        std::string tbname;
        std::string one;
        //解析行
        std::stringstream ss(line);
        ss >> username >> tbname;
        std::vector<std::string> lis;
        while(ss >> one){
            lis.push_back(one);
        }
        //找到记录中的相关权限
        DBUser::UserInfo info;
        if(this->_mapper.find(username) != this->_mapper.end()){
            info = this->_mapper[username];
        }
        //权限设置
        info.user = username;
        info.addRight(tbname,lis);
        //更新权限
        this->_mapper[username] = info;
    }
    file.close();
    printf("read1 over\n");
    //读取账户密码信息
    line = "";
    file.open(this->usr_path,std::ios::in);
    while(getline(file,line)){
        int lenth = line.size();
        if(lenth == 0) continue;
        //注释行
        if(line[0] == '#') continue;
        //解析语句
        for(int i=0;i<lenth;i++){
            if(line[i] == ':'){
                line[i] = ' ';
                break;
            }
        }
        printf("user-pass-line:%s\n",line.c_str());
        std::stringstream ss(line);
        std::string username;
        std::string password;
        ss >> username >> password;
        //设置系统信息
        std::map<std::string,DBUser::UserInfo>::iterator iter;
        iter = this->_mapper.find(username);
        if(iter != this->_mapper.end()){
            iter->second.password = password;
        }
    }
    file.close();
    printf("read2 over\n");
}

//数据写出
void DBUser::UserChecker::WriteFile(){
    //打开用户.usr文件
    std::fstream file;
    file.open(this->usr_path,std::ios::out);
    //输出用户账户密码信息
    std::map<std::string,DBUser::UserInfo>::iterator iter;
    iter = this->_mapper.begin();
    while(iter != this->_mapper.end()){
        file << iter->second.user << " " << iter->second.password << "\n";
        iter++;
    }
    file.close();
    //打开用户.right管理文件
    file.open(this->right_path,std::ios::out);
    //输出账户权限信息
    iter = this->_mapper.begin();
    while(iter != this->_mapper.end()){
        file << iter->second.generateRight2String() << "\n";
        iter++;
    }
    file.close();
}

//权限查询
int DBUser::UserChecker::admit(const std::string username,const std::string tbname,const std::string one_right){
    std::map<std::string,DBUser::UserInfo>::iterator iter;
    iter = this->_mapper.find(username);
    //没有用户信息
    if(iter == this->_mapper.end()) return 0;
    //检查权限许可
    int res = iter->second.hasRightonTable(tbname,one_right);
    return res;
}

void DBUser::UserChecker::show(){
    std::map<std::string,DBUser::UserInfo>::iterator iter;
    iter = this->_mapper.begin();
    while(iter != this->_mapper.end()){
        printf("[username]:%s [password]:%s\n",iter->second.user.c_str(),iter->second.password.c_str());
        iter->second.show();
        iter++;
    }
}

//需要引用空用户变量
DBUser::UserInfo null_user;

DBUser::UserInfo& DBUser::UserChecker::getUser(const std::string username,int& res){
    std::map<std::string,DBUser::UserInfo>::iterator iter;
    iter = this->_mapper.find(username);
    if(iter != this->_mapper.end()){
        DBUser::UserInfo& tp = iter->second;
        if(tp.user == username){
            res = 1;
            return iter->second;//注意这里是引用
        }
    }
    res = -1;
    return null_user;//返回一个空引用变量
}

//授权
int DBUser::UserChecker::grantRights(const std::string one_from,const std::string one_to,const std::string tbname,std::vector<std::string> rights){
    int ret = 0;
    UserInfo& usr_from = this->getUser(one_from,ret);
    if(ret < 0) { return -1; }
    ret = 0;
    UserInfo& usr_to = this->getUser(one_to,ret);
    if(ret < 0) { return -1; }
    //授权
    return usr_from.grantRight(usr_to,tbname,rights);
}

//收回多个权限
int DBUser::UserChecker::revokeRights(const std::string one_from,const std::string one_to,const std::string tbname,std::vector<std::string> rights){
    int ret = 0;
    UserInfo& usr_from = this->getUser(one_from,ret);
    if(ret < 0) { return -1; }
    ret = 0;
    UserInfo& usr_to = this->getUser(one_to,ret);
    if(ret < 0) { return -1; }
    //收回权限
    return usr_from.revokeRight(usr_to,tbname,rights);
}

#endif //_DBUSER_CPP_


