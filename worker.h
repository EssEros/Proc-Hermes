#ifndef WORKER_H
#define WORKER_H

#include "Common.h"
#include <winsock2.h>
#include <QDebug>
#include <string>
#include <unistd.h>
//#include <QList>
#include "filemanager.h"
#include <QStringList>
#include <QMessageBox>
#include <QMap>
#include <atomic>
#include <QThread>
#include <mutex>
#include <QQueue>
#include <QObject>


class Worker : public QObject
{
    Q_OBJECT
public:
    explicit Worker(int clntSock, QObject *parent = nullptr);

signals:
    void chatMessRecved(Record record);
    void applyMessRecvd(QString appName);
    void userExist();
    void userNotExist();
    void userAppRejected(QString rejName);
    void userAppAgreed(QString friendName);

public slots:
    void listener();
//    void closeListen();

public:
    std::atomic<bool> userIsOnline;     // 原子变量，用于控制线程的"启动"与"终止"
    std::atomic<bool> userNotClosed;    // 原子变量，如上
    int clntSock;                      // 接收的套接字

    void recvData(QString &data);        // 接收信息
};

#endif // WORKER_H
