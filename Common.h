#ifndef COMMON_H
#define COMMON_H

#include <string>
#include <list>
#include <QString>

using std::string;
using std::list;

namespace proj {
    // 注册账号
    const string Register = "SIGNUPACCOUNT";
    // 注册失败，用户存在于库中
    const string UserNameIsExist = "USERNAMEISEXIST";
    // 注册成功
    const string UserNameIsNotExist = "USERNAMEISNOTEXIST";
    // 登陆账号
    const string Login = "SIGNINACCOUNT";
    // 密码正确
    const string PasswordIsCorrect = "PASSWORDISCORRECT";
    // 密码错误
    const string PasswordIsWrong = "PASSWORDISWRONG";
    // 返回
    const string Return = "RETURNMESSAGE";
    // 确认
    const string Confirm = "CONFIRMMESSAGE";
    // 拒绝
    const string Refuse = "REFUSEMESSAGE";
    // 好友列表1
    const string FriendList1 = "FRIENDLIST1";
    // 好友列表2
    const string FriendList2 = "FRIENDLIST2";
    // 我的信息
    const string SelfMessage = "SELFMESSAGE";
    // 和好友聊天
    const string ChatWithFriend = "CHATWITHFRIEND";
    // 信息包
    const string MessagePack = "MESSAGEPACK";
    // 关闭包
    const string Close = "CLOSEMESSAGE";
    // 直接退出
    const string CloseStraightly = "CLOSESTRAIGHTLY";
    // 信息为空
    const string MessageIsEmpty = "MESSAGEISEMPTY";
    // 进入主页
    const string IntoMainPage = "INTOMAINPAGE";
    // 同意申请
    const string AgreeApplication = "AGREEAPPLICATION";
    // 申请信息
    const string ApplicationMessage = "APPLICATIONMESSAGE";
    // 拒绝
    const string RejectMessage = "REJECTMESSAGE";
    // 申请被拒绝
    const string ApplyRejected = "APPLYREJECTED";
    // 传输信号的标准长度
    const int MessLen = 100;
    // 传输数据的标准长度
    const int DataLen = 1024;
}


struct Record {
    QString sender;
    QString message;

    Record(QString sender = "", QString message = ""): sender(sender), message(message){}
};


struct User {
    string userName;
    string passWord;
    bool isOnline;

    User(const string&userName = "", const string &passWord = "", const bool isOnline = false): userName(userName), passWord(passWord), isOnline(isOnline){}
    // 移动构造函数
    User(User && user): userName(std::move(user.userName)), passWord(std::move(user.passWord)), isOnline(std::move(user.isOnline)){}
    // 拷贝构造函数
    User& operator=(const User &user){
        if(this != &user){
            this->userName = user.userName;
            this->passWord = user.passWord;
            this->isOnline = user.isOnline;
        }
        return *this;
    }
};

#endif // COMMON_H
