#ifndef MANAGER_H
#define MANAGER_H

#include <QObject>
#include "Common.h"
#include <winsock2.h>
#include <QDebug>
#include <string>
#include <unistd.h>
//#include <QList>
#include "filemanager.h"
#include <QStringList>
#include <QThread>
#include <QMap>
#include "worker.h"
#pragma comment(lib, "ws2_32.lib")

using std::string;

class Manager : public QObject
{
    Q_OBJECT
private:
    bool __isConnect;       // 判断初始化是否成功
    SOCKET clntSock;           // 客户端套接字

public:
    User *Me;               // 用户信息
    QStringList *friendList;     // 好友列表
    QStringList *applyToMeList;  // 对我的申请者列表
    QStringList *applyToOthersList;  // 对其他人的申请者列表
    QMap<QString, QList<Record>> *tmpChatRecord;  // 聊天记录

public:
    explicit Manager(const string &ip_addr, const int &port, QObject *parent);
    ~Manager();
    void Connect(const string &ip_addr, const int &port);   // 连接服务器
    inline bool isConnect(){ return this->__isConnect; } // 判断是否连接成功
    void DisConnect();      // 断开连接
    void sendData(const string &data);    // 向服务端传输用户的数据
    void sendMess(const string &message);   // 向服务端发送用户的信息
    void recvMess(string &message);              // 从服务端获取回应信息，以确认当前状态
    void recvData(string &data, size_t dataLen = proj::DataLen);            // 从服务端获取数据
    void setMyName(const string &myName);       // 设置我的名字
    string getMyName();                         // 获取我的名字
    void setMyPassword(const string &myPassword);// 设置我的密码
    void getMyFriendList(QStringList *myFriendList);           // 获取我的好友列表
    void getApplyToMeList(QStringList *applyToMeList);          // 获取对我的申请者列表
    void getApplyToOthersList(QStringList *applyToOthersList);      // 获取对他人的申请列表

    // 对列表的增删查改
    void initFriList();      // 初始化好友列表
    void initAppToMeList();     // 初始化申请列表
    void initAppToOtherList();  // 初始化我的申请列表
    void initTmpChatRecord();      // 对聊天记录初始化
    bool userRegister();            // 用户注册
    bool checkUser();               // 检测用户的信息

    QThread *threadRecv;
    Worker *worker;

signals:
    void newMessRecved(Record record);
    void newApplyRecved(QString appName);
    void applySuccess();
    void applyFailed();
    void userAppRejected(QString rejName);       // 转发用户的申请被拒绝
    void userAppAgreed(QString friendName);     // 转发用户的申请被同意的消息
    void refreshList();                   // 更新列表
    //    void

public slots:
};

extern Manager manager;

#endif // MANAGER_H
