#ifndef REGISTERPAGE_H
#define REGISTERPAGE_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QMessageBox>
#include <QString>
#include <QVBoxLayout>
#include "manager.h"

class RegisterPage : public QWidget
{
    Q_OBJECT
public:
    explicit RegisterPage(QWidget *parent = nullptr);
    ~RegisterPage();
signals:
    void registerSuccess();        // 登陆成功后发出的信号
    void registerFailed();         // 登陆失败后发出的信号
    void returnClicked();          // 返回按钮被点击

public slots:
    void onUserNameInputed();   // 用户账号被输入
    void onPassWordInputed();   // 用户密码被输入
    void onRegisterClicked();   // 对登陆按钮被点击事件的处理
    void onReturnClicked();     // 当返回按钮被点击

private:
    // 用于页面交互
    QLineEdit *userNameEdit;    // 用户账号输入栏
    QLineEdit *passWordEdit;    // 用户密码输入栏
    QPushButton *registerButton;// 注册按钮
    QPushButton *returnButton;  // 返回按钮
};

#endif // REGISTERPAGE_H
