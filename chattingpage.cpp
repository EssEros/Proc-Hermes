#include "chattingpage.h"

// 待完成的内容，数据持久化（保存到文件里，以及处理退出界面后记录消失的问题）
ChattingPage::ChattingPage(QWidget *parent) : QWidget(parent)
{
    QString friendName = "";
    qDebug() << friendName;
    this->messList = new QStringList;

    QVBoxLayout *layout = new QVBoxLayout(this);
    QHBoxLayout *topLayout = new QHBoxLayout(this);

    this->returnButton = new QPushButton(this);
    this->friendName = new QLabel(this);
    this->messageList = new QListWidget(this);
    this->inputArea = new QLineEdit(this);
    this->commitButton = new QPushButton(this);


    this->returnButton->setText("返回");
    this->friendName->setText(friendName);
    this->messageList->addItems(*this->messList);
    this->inputArea->setPlaceholderText("请输入...");
    this->commitButton->setText("确认发送");

    topLayout->addWidget(this->returnButton, 2);
    topLayout->addWidget(this->friendName, 8);
    layout->addLayout(topLayout);
    layout->addWidget(this->messageList, 7);
    layout->addWidget(this->inputArea, 2);
    layout->addWidget(this->commitButton, 2);

    // 当输入完毕后将输入框中的值存下来
    QObject::connect(this->inputArea, &QLineEdit::editingFinished, this, &ChattingPage::WriteIntoSendMess);
    // 按下按钮后发送数据
    QObject::connect(this->commitButton, &QPushButton::clicked, this, &ChattingPage::sendMessage);
    // 按下回车后发送数据
    QObject::connect(this->inputArea, &QLineEdit::returnPressed, this, &ChattingPage::sendMessage);
    // 返回被点击
    QObject::connect(this->returnButton, &QPushButton::clicked, this, &ChattingPage::returnClicked);
    // 有新的消息到来
    QObject::connect(&manager, &Manager::newMessRecved, this, &ChattingPage::updateMessList);
}

ChattingPage::~ChattingPage()
{

}

// 发送数据
void ChattingPage::sendMessage()
{
    this->WriteIntoSendMess();
    string message = proj::ChatWithFriend + "|" + manager.Me->userName + "|" + friendName->text().toStdString() + "|" + this->sendMess + "|";
    manager.sendMess(message);
    this->inputArea->setText("");
    this->messList->push_back(QString(this->sendMess.c_str()));
    this->messageList->addItem(QString(this->sendMess.c_str()));
}

// 获取输入框中的值
void ChattingPage::WriteIntoSendMess()
{
    this->sendMess = this->inputArea->text().toStdString();
}

// 更新消息列表
void ChattingPage::updateMessList(Record record)
{
    qDebug() << "有新消息到了";
    if(record.message.length() > 0)
    {
        qDebug() << record.sender << " 发送了新消息 " << record.message;
        this->messList->push_back(record.message);
        this->messageList->addItem(record.message);
    }
}

// 返回被点击
void ChattingPage::returnClicked()
{
    qDebug() << "用户退出聊天";
    emit sendReturnSignal();
}
