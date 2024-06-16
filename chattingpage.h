#ifndef CHATTINGPAGE_H
#define CHATTINGPAGE_H

#include <QWidget>
#include <QString>
#include <QVBoxLayout>
#include <QLabel>
#include <QListWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QThread>
#include <QHBoxLayout>
#include "manager.h"

class ChattingPage : public QWidget
{
    Q_OBJECT
public:
    explicit ChattingPage(QWidget *parent = nullptr);
    ~ChattingPage();
signals:
    void sendMessSignal(QString mess);
    void sendReturnSignal();

public slots:
    void sendMessage();                  // 发送数据
    void WriteIntoSendMess();         // 获取输入框中的值
    void updateMessList(Record record);  // 更新消息列表
    void returnClicked();               // 返回被点击后发送信号

public:
    QStringList *messList;
    string sendMess;
    string recvMess;

    QPushButton *returnButton;
    QLabel *friendName;
    QListWidget *messageList;
    QLineEdit *inputArea;
    QPushButton *commitButton;

    QThread *threadRecv;
};

#endif // CHATTINGPAGE_H
