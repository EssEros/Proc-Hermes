#include "homepage.h"

HomePage::HomePage(QWidget *parent)
    : QWidget{parent}
{
    // 初始化
    QVBoxLayout *layout = new QVBoxLayout(this);
    this->login = new QPushButton("登陆", this);
    this->register_ = new QPushButton("注册", this);

    // 向 QVBoxLayout 中添加组件
    layout->addWidget(this->login);
    layout->addWidget(this->register_);

    // 连接信号与槽
    // 点击完按钮后通过槽函数发送信号
    QObject::connect(this->login, &QPushButton::clicked, this, &HomePage::onLoginClicked);
    QObject::connect(this->register_, &QPushButton::clicked, this, &HomePage::onRegisterClicked);
}

// 后续可以尝试将其改为信号函数，通过发送信号告知mainwindow用户选了什么选项
void HomePage::onLoginClicked()
{
    qDebug() << "用户点击了登陆";
    emit this->loginClicked();
}

void HomePage::onRegisterClicked()
{
    qDebug() << "用户点击了注册";
    emit this->registerClicked();
}

