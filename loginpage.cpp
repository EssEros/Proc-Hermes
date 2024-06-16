#include "loginpage.h"

LoginPage::LoginPage(QWidget *parent)
    : QWidget{parent}
{
    // 初始化
    this->userNameEdit = new QLineEdit(this);
    this->passWordEdit = new QLineEdit(this);
    this->loginButton = new QPushButton("登陆", this);
    this->returnButton = new QPushButton("返回", this);

    // 对控件进行自定义
    this->userNameEdit->setPlaceholderText("请输入您的账号: ");
    this->userNameEdit->setMaxLength(30);
    this->passWordEdit->setPlaceholderText("请输入您的密码: ");
    this->passWordEdit->setMaxLength(30);
    this->passWordEdit->setEchoMode(QLineEdit::Password);

    // 对输入框和按钮进行垂直布局
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(this->userNameEdit);
    layout->addWidget(this->passWordEdit);
    layout->addWidget(this->loginButton);
    layout->addWidget(this->returnButton);

    // 连接信号与槽
    // 下一个版本在增加判断账号密码是否符合标准的功能
    // 当用户账号输入结束或是焦点转移
    QObject::connect(userNameEdit, &QLineEdit::returnPressed, this, &LoginPage::onUserNameInputed);
    QObject::connect(userNameEdit, &QLineEdit::editingFinished, this,  &LoginPage::onUserNameInputed);
    // 当用户密码输入结束
    QObject::connect(passWordEdit, &QLineEdit::returnPressed, this, &LoginPage::onPassWordInputed);
    QObject::connect(passWordEdit, &QLineEdit::editingFinished, this, &LoginPage::onPassWordInputed);
    // 当登陆按钮被点击
    QObject::connect(loginButton, &QPushButton::clicked, this, &LoginPage::onLoginClicked);
    // 返回按钮被点击
    QObject::connect(returnButton, &QPushButton::clicked, this, &LoginPage::onReturnClicked);
}

void LoginPage::onUserNameInputed()
{
    QString userName = this->userNameEdit->text();
    manager.setMyName(userName.toStdString());
}

void LoginPage::onPassWordInputed()
{
    QString passWord = this->passWordEdit->text();
    manager.setMyPassword(passWord.toStdString());
}

void LoginPage::onReturnClicked()
{
    qDebug() << "从 loginpage 退回到 homepage";
    emit this->returnClicked();
}


void LoginPage::onLoginClicked()
{
    // 接收服务器检测结果
    bool isCorrect = manager.checkUser();
    if(isCorrect)
    {
        // 分别按序接收客户端发来的好友列表、好友申请列表、我的申请列表、聊天记录
        emit loginSuccess();
    }
    else
    {
        qDebug() << "用户登陆失败";
        emit loginFailed();
    }
}
