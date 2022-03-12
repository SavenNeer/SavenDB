//dbuser.h

#ifndef _DBUSER_H_
#define _DBUSER_H_

#include<string>
#include<vector>
#include<map>

namespace DBUser{


    /* 用户基本信息
     * 包括用户 账户 密码 权限集合
    */
    class UserInfo{
    public:
        //用户账户
        std::string user;
        std::string password;
        //被允许的权限-需要从权限文件装载
        std::map<std::string,std::map<std::string,int> > rights;
        //是否对某些表具有权限
        std::map<std::string,int> hasRight;
    public:
        //构建函数
        UserInfo();
    public:
        //设置密码
        void setPassword(const std::string password);
        //添加账户权限(组)
        void addRight(const std::string tbname,std::vector<std::string> rhs);
        //删除账户上的指定权限
        void delRight(const std::string tbname,std::vector<std::string> rhs);
        //检查账户权限
        int hasRightonTable(const std::string tbname,const std::string right);
        //用户授权 成功0 失败-1
        int grantRight(UserInfo& user,const std::string tbname,const std::vector<std::string> rhs);
        //用户收权
        int revokeRight(UserInfo& user,const std::string tbname,const std::vector<std::string> rhs);
        //生成向系统文件.right写入的账户信息字符串
        std::string generateRight2String();
        //show
        void show();
    };



    /* 数据库账户与权限管理 */
    class UserChecker{
    public:
        //存储地址
        const std::string usr_path = "../data/db.usr";
        const std::string right_path = "../data/db.right";
        //用户信息集合-直接获取用户信息
        std::map<std::string,DBUser::UserInfo> _mapper;
    public:
        UserChecker();
        ~UserChecker();
    public:
        //权限和用户文件的读写
        void ReadFile();
        void WriteFile();
    public:
        //单个权限查询
        int admit(const std::string username,const std::string tbname,const std::string one_right);
        //show
        void show();
        //获取用户信息
        //存在用户返回1 不存在用户则返回-1
        UserInfo& getUser(const std::string username,int& res);
        //授多个权限
        int grantRights(const std::string one_from,const std::string one_to,const std::string tbname,std::vector<std::string> rights);
        //收回多个权限
        int revokeRights(const std::string one_from,const std::string one_to,const std::string tbname,std::vector<std::string> rights);
    };

};



#endif //_DBUSER_H_


