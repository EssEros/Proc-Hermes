#include "registerpage.h"

RegisterPage::RegisterPage(QWidget *parent)
    : QWidget{parent}
{
    // 初始化
    this->userNameEdit = new QLineEdit(this);
    this->passWordEdit = new QLineEdit(this);
    this->registerButton = new QPushButton("注册", this);
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
    layout->addWidget(this->registerButton);
    layout->addWidget(this->returnButton);

    // 连接信号与槽
    // 当用户账号输入结束
    QObject::connect(userNameEdit, &QLineEdit::returnPressed, this, &RegisterPage::onUserNameInputed);
    QObject::connect(userNameEdit, &QLineEdit::editingFinished, this,  &RegisterPage::onUserNameInputed);
    // 当用户密码输入结束
    QObject::connect(passWordEdit, &QLineEdit::returnPressed, this, &RegisterPage::onPassWordInputed);
    QObject::connect(passWordEdit, &QLineEdit::editingFinished, this, &RegisterPage::onPassWordInputed);
    // 当用户按钮被点击
    QObject::connect(this->registerButton, &QPushButton::clicked, this, &RegisterPage::onRegisterClicked);
    // 当返回按钮被点击
    QObject::connect(this->returnButton, &QPushButton::clicked, this, &RegisterPage::onReturnClicked);
}

void RegisterPage::onUserNameInputed()
{
    QString userName = this->userNameEdit->text();
    manager.setMyName(userName.toStdString());
}


void RegisterPage::onPassWordInputed()
{
    QString passWord = this->passWordEdit->text();
    manager.setMyPassword(passWord.toStdString());
}

void RegisterPage::onRegisterClicked()
{
    bool isCorrect = manager.userRegister();
    if(isCorrect)
    {
        qDebug() << "用户注册成功";
        emit registerSuccess();
    }
    else
    {
        qDebug() << "用户注册失败";
        emit registerFailed();
    }
}

void RegisterPage::onReturnClicked()
{
    // 发送返回按钮被点击的信号
    qDebug() << "从 registerpage 返回到 homepage";
    emit this->returnClicked();
}

RegisterPage::~RegisterPage()
{
    delete userNameEdit;
    delete passWordEdit;
    delete registerButton;
}
