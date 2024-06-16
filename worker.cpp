#include "worker.h"

Worker::Worker(int clntSock, QObject *parent) : QObject(parent), clntSock(clntSock)
{
    this->userIsOnline.store(false);
    this->userNotClosed.store(true);
}

void Worker::listener(){
    // 监听服务器的信息
    QString data, fri, me, mess;
    while(this->userNotClosed.load()){
        if(userIsOnline.load())
        {
            this->recvData(data);
            qDebug() << "接收到消息: " << data;
            // 将数据解除序列化
            QStringList record = data.split("|", QString::SkipEmptyParts);
            auto it = record.begin();

            // 若是聊天信息
            // proj::ChatWithFriend|friendName|userName|mess|
            if(it->toStdString() == proj::ChatWithFriend)
            {
                fri = *(++it); me = *(++it); mess = *(++it);
                qDebug() << fri << " 向我发送了 " << mess;
                // 发送信息：将消息转发、告知是消息包
                emit chatMessRecved(Record(fri, mess));
            }
            // 若是申请信息
            // proj::ApplicationMessage|otherName|userName|
            else if (it->toStdString() == proj::ApplicationMessage)
            {
                fri = *(++it);
                emit applyMessRecvd(fri);
            }
            // 所申请的用户不存在
            else if (it->toStdString() == proj::UserNameIsNotExist)
            {
                emit userNotExist();
            }
            // 所申请的用户存在
            else if (it->toStdString() == proj::UserNameIsExist)
            {
                emit userExist();
            }
            // 申请被拒绝
            // proj::ApplyRejected|rejName|
            else if (it->toStdString() == proj::ApplyRejected)
            {
                QString rejName = *(++it);
                qDebug() << "用户对 " << rejName << " 的申请被拒绝";
                // 发送信号，让 manager 接收
                emit userAppRejected(rejName);
            }
            // 申请被同意
            // proj::AgreeApplication|friendName|userName|
            else if (it->toStdString() == proj::AgreeApplication)
            {
                QString friendName = *(++it);
                qDebug() << "用户对 " << friendName << " 的申请被同意";
                // 发送申请被同意的信息
                emit userAppAgreed(friendName);
            }
        }
    }
}

void Worker::recvData(QString &Data)
{
    char data[proj::MessLen] = {0};
    recv(this->clntSock, data, proj::DataLen, 0);
    Data = QString(data);
}
