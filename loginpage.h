#ifndef LOGINPAGE_H
#define LOGINPAGE_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QString>
#include "manager.h"

class LoginPage : public QWidget
{
    Q_OBJECT
public:
    explicit LoginPage(QWidget *parent = nullptr);

signals:
    void loginSuccess();        // 登陆成功后发出的信号
    void loginFailed();         // 登陆失败后发出的信号
    void returnClicked();       // 返沪按钮被点击后发出的信号
    void recvAndupdateAllLists();   // 对所有列表进行更新

public slots:
    void onUserNameInputed();   // 用户账号被输入
    void onPassWordInputed();   // 用户密码被输入
    void onLoginClicked();      // 对登陆按钮被点击事件的处理
    void onReturnClicked();     // 对返回按钮被点击事件的处理

private:
    // 用于页面交互
    QLineEdit *userNameEdit;    // 用户账号输入栏
    QLineEdit *passWordEdit;    // 用户密码输入栏
    QPushButton *loginButton;   // 登陆按钮
    QPushButton *returnButton;  // 返回按钮
};

#endif // LOGINPAGE_H
