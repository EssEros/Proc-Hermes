#include "manager.h"
#include <QMessageBox>

// 构造函数
// 完成对自身 sock 的初始化
Manager::Manager(const string &ip_addr, const int &port, QObject *parent = nullptr): QObject(parent)
{
    // 初始化
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);
    this->Me = new User;
    this->friendList = new QStringList;
    this->applyToMeList = new QStringList;
    this->applyToOthersList = new QStringList;
    this->tmpChatRecord = new QMap<QString, QList<Record>>;
    this->clntSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(this->clntSock == INVALID_SOCKET)
    {
        qDebug() << "创建客户端套接字失败";
        this->__isConnect = false;
        return;
    }
    // 连接服务器
    this->Connect(ip_addr, port);
    if(this->isConnect())
    {
        qDebug() << "连接服务器成功";
    }

    this->threadRecv = new QThread(this);
    this->worker = new Worker(this->clntSock);
    worker->moveToThread(this->threadRecv);

    // 创建线程后执行监听任务
    QObject::connect(threadRecv, &QThread::started, worker, &Worker::listener);

    // chatMessRecved applyMessRecvd userNotExist userExist
    // 接收到聊天信息
    QObject::connect(worker, &Worker::chatMessRecved, this, [=](Record record){
        // 发送信息通知更新
        (*tmpChatRecord)[record.sender].push_back(record);
        // 通知更新
        emit newMessRecved(record);
    });

    // 接收到新的好友申请
    QObject::connect(worker, &Worker::applyMessRecvd, this, [=](QString appName){
        // 添加记录
        applyToMeList->push_back(appName);
        // 通知更新
        emit newApplyRecved(appName);
    });

    // 接收到申请失败的信息
    QObject::connect(worker, &Worker::userNotExist, this, [=](){
        // 转发
        qDebug() << "申请的好友不存在";
        emit applyFailed();
    });

    // 接收到申请成功的信息
    QObject::connect(worker, &Worker::userExist, this, [=](){
        qDebug() << "申请成功";
        emit applySuccess();
    });
    // 用户的申请被拒绝
    QObject::connect(worker, &Worker::userAppRejected, this, [=](QString rejName){
       qDebug() << "删除 manager 中对 " << rejName << " 的申请记录";
       this->applyToOthersList->removeOne(rejName);
       // 向 applyPage 转发被拒绝的信号
       emit this->userAppRejected(rejName);
    });
    // 用户的申请被同意
    QObject::connect(worker, &Worker::userAppAgreed, this, [=](QString friendName){
        // 更新列表
        this->applyToOthersList->removeOne(friendName);
        this->friendList->push_back(friendName);
        // 加入好友列表并通知更新
        emit this->userAppAgreed(friendName);
    });
    this->threadRecv->start();
}

Manager::~Manager()
{
    qDebug() << "关闭套接字: " << this->clntSock;
    delete this->Me;
    delete this->friendList;
    delete this->applyToMeList;
    delete this->applyToOthersList;
    delete this->worker;
    closesocket(this->clntSock);
    WSACleanup();
}

// 连接服务器
void Manager::Connect(const string &ip_addr, const int &port)
{
    SOCKADDR_IN servAddr;
    memset(&servAddr, 0, sizeof(servAddr));
    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.S_un.S_addr = inet_addr(ip_addr.c_str());
    servAddr.sin_port = htons(3333);
    // 连接服务端
    if(::connect(clntSock, (sockaddr*)&servAddr, sizeof(sockaddr)) == -1)
    {
        qDebug() << "连接服务端失败";
        this->__isConnect = false;
        this->DisConnect();
        return;
    }
    this->__isConnect = true;
}

// 断开连接
void Manager::DisConnect()
{
    // 关闭监听线程
    manager.worker->userIsOnline.store(false);
    manager.worker->userNotClosed.store(false);
    this->threadRecv->quit();
    this->threadRecv->wait();
    // 断开连接
    shutdown(this->clntSock, SD_BOTH);
    // 关闭套接字
    closesocket(this->clntSock);
    WSACleanup();
}

// 设置我的名字
void Manager::setMyName(const string &myName)
{
    this->Me->userName = myName;
}

// 获取我的名族
string Manager::getMyName()
{
    return this->Me->userName;
}

// 设置我的密码
void Manager::setMyPassword(const string &myPassword)
{
    this->Me->passWord = myPassword;
}

// 获取我的好友列表
void Manager::getMyFriendList(QStringList *myFriendList)
{
    myFriendList = this->friendList;
}

// 获取对我的申请者列表
void Manager::getApplyToMeList(QStringList *applyToMeList)
{
    applyToMeList = this->applyToMeList;
}

// 获取对他人的申请列表
void Manager::getApplyToOthersList(QStringList *applyToOthersList)
{
    applyToOthersList = this->applyToOthersList;
}

// 向服务端传输用户的数据
void Manager::sendData(const string &data)
{
    size_t len = send(this->clntSock, data.c_str(), data.length()+1, 0);
    if(len < 0)
    {
        qDebug() << "数据序列发送失败";
    }
}

// 向服务端发送用户的信息
void Manager::sendMess(const string &mess)
{
    size_t len = send(this->clntSock, mess.c_str(), mess.length()+1, 0);
    if(len < 0)
    {
        qDebug() << "信息序列发送失败";
    }
}

// 从服务器获取回应信息，以确认当前状态
void Manager::recvMess(string &message)
{
    char mess[proj::MessLen] = {0};
    recv(this->clntSock, mess, proj::MessLen, 0);
    message = mess;
}

// 从服务端获取数据
void Manager::recvData(string &data, size_t dataLen)
{
    char _data[proj::DataLen] = {0};
    if(dataLen > 10*proj::DataLen)
        qDebug() << "数据长度超过限制";
    recv(this->clntSock, _data, dataLen, 0);
    data = _data;
}

bool Manager::userRegister()
{
    string message;
    // 将用户名和密码序列化一起发送
    string userSequence = proj::Register + "|" + this->Me->userName + "|" + this->Me->passWord + "|";
    // 发送序列
    this->sendData(userSequence);
    // 接收服务端回应
    this->recvMess(message);
    if(message == proj::UserNameIsNotExist)
    {
        qDebug() << "登陆成功";
        return true;
    }
    else
    {
        qDebug() << "用户名 " << QString(this->Me->userName.c_str()) << " 已存在";
        return false;
    }
}

bool Manager::checkUser()
{
    string message;
    // 将用户名和密码序列化一起发送
    string userSequence = proj::Login + "|" + this->Me->userName + "|" + this->Me->passWord + "|";
    // 发送序列
    this->sendData(userSequence);
    // 接收服务端回应
    this->recvMess(message);
    if(message == proj::PasswordIsCorrect){
        qDebug() << "登陆成功";
        return true;
    }
    else
    {
        qDebug() << "登陆失败";
        return false;
    }
}

// 初始化好友列表
void Manager::initFriList()
{
    string data;
    qDebug() << "接收好友列表数据";
    this->recvData(data);
    // 发送确认信息
    this->sendMess(proj::Confirm);
    // 对信息解除序列化
    *this->friendList << QString(data.c_str()).split("|", QString::SkipEmptyParts);
}

// 初始化申请列表
void Manager::initAppToMeList()
{
    string data;
    qDebug() << "接收好友申请列表数据";
    this->recvData(data);
    // 发送确认信息
    this->sendMess(proj::Confirm);
    // 对信息解除序列化
    *this->applyToMeList << QString(data.c_str()).split("|", QString::SkipEmptyParts);
}

// 初始化我的申请列表
void Manager::initAppToOtherList()
{
    string data;
    qDebug() << "接收我的申请列表数据";
    this->recvData(data);
    // 发送确认信息
    this->sendMess(proj::Confirm);
    // 对信息解除序列化
    *this->applyToOthersList << QString(data.c_str()).split("|", QString::SkipEmptyParts);
}

// 初始化临时聊天记录
void Manager::initTmpChatRecord()
{
    string record;
    qDebug() << "接收临时聊天记录";
    this->recvData(record, 2*proj::DataLen);
    qDebug() << QString(record.c_str());
    this->sendMess(proj::Confirm);
    // 先对信息解除序列化
    QStringList tmp;
    tmp << QString(record.c_str()).split("|", QString::SkipEmptyParts);
    // 分组将信息存入临时消息记录中
    QString sender, recivier, mess;
    for(auto i = tmp.begin(); tmp.length() >= 3 && i != tmp.end(); i+=3)
    {
        sender = *i;
        recivier = *(i+1);
        mess = *(i+2);
        // 若发送者是我
        if(QString(this->Me->userName.c_str()) == sender)
        {
            (*this->tmpChatRecord)[recivier].push_back(Record(sender, mess));
        }
        else if(QString(this->Me->userName.c_str()) == recivier)
        {
            (*this->tmpChatRecord)[sender].push_back(Record(sender, mess));
        }
    }
}


Manager manager("47.97.173.118", 3333);
