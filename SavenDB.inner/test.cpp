#include"tbcore.h"
#include"test_table.h"
#include<iostream>
#include"stdio.h"

using namespace std;

typedef vector<vector<string> > TBlis;

//命令读取
string readOrder(){
    string all = "";
    while(1){
        printf("[SavenDB] >");
        string part = "";
        getline(cin,part);
        int lenth = part.size();
        int flag = 0;
        for(int i=0;i<lenth;i++){
            char ch = part[i];
            all += ch;
            if(ch == ';'){
                flag = 1;
                break;
            }
        }
        if(flag) break;
    }
    return all;
}


//输出结果
void output(TBlis& rhs){
    cout << "output():" << endl;
    TableUI::TextTable p;
    int res = TableUI::convertTextTable(rhs,p);
    cout << "convert res = " << res << endl;
    cout << p << endl;
    // int lenth = rhs.size();
    // for(int i=0;i<lenth;i++){
    //     vector<string>& tp = rhs[i];
    //     for(int k=0;k<tp.size();k++){
    //         cout << tp[k] << " ";
    //     }
    //     cout << endl;
    // }
    // cout << " == result == " << endl;
}

vector<string> sqlist = {
    "CREATE TABLE S(SNO CHAR(2) PRIMARY KEY,SNAME VARCHAR(20) NOT NULL,STATUS INT UNIQUE,CITY VARCHAR(20));",
    "insert into S(SNO,SNAME,STATUS,CITY) VALUES (\"S1\",\"jingyi\",20,\"tianjin\");",
    "insert into S(SNO,SNAME,STATUS,CITY) VALUES (\"S1\",\"hahad\",50,\"tianjin\");",
    "insert into S(SNO,SNAME,STATUS,CITY) VALUES (\"S2\",\"shengxi\",10,\"beijing\");",
    "insert into S(SNO,SNAME,STATUS,CITY) VALUES (\"S3\",\"dongfang\",30,\"beijing\");",
    "select * from S;"
};

int main(){
    TBCore::SavenDB_TBCore sysCore;
    TBlis res;

    while(1){
        string order = readOrder();
        if(order == "quit;"){
            cout << "SavenDB Over" << endl;
            break;
        }
        cout << "order=> " << order << endl;
        res.clear();
        int ret = sysCore.check("test.tb",order);
        cout << "run-return-value = " << ret << endl;
        if(order.find("select") != order.npos){
            res = sysCore.res;
            output(res);
        }
        cout << "=== once ok ===" << endl;
    }
    
    // TBlis res;
    // int lenth = sqlist.size();
    // for(int i=0;i<lenth;i++){
    //     cout << "cnt = " << i << endl;
    //     string order = sqlist[i];
    //     cout << "order=> " << order << endl;
    //     res.clear();
    //     int ret = sysCore.check("test.tb",order);
    //     cout << "ret = " << ret << endl;
    //     if(order.find("select") != order.npos){
    //         res = sysCore.res;
    //         output(res);
    //     }
    //     cout << "once ok" << endl;
    //     cout << endl;
    // }

    // string order = "";
    // while(1){
    //     order = "";
    //     string one = "";
    //     while(getline(cin,one)){
    //         int lenth = one.size();
    //         int flag = 0;
    //         for(int i=0;i<lenth;i++){
    //             order += one[i];
    //             if(one[i] == ';'){
    //                 flag = 1;
    //             }
    //         }
    //         if(flag) break;
    //     }
    //     //
    //     sysCore.check("test.tb",order);
    //     if(order.find("select") != order.npos){
    //         output(sysCore.res);
    //     }

    //     cout << endl;
    // }

    return 0;
}




