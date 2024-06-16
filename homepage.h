#ifndef HOMEPAGE_H
#define HOMEPAGE_H

#include <QWidget>
#include <QVBoxLayout>
#include <QPushButton>
#include "loginpage.h"
#include "registerpage.h"

class HomePage : public QWidget
{
    Q_OBJECT
public:
    explicit HomePage(QWidget *parent = nullptr);

signals:
    void loginClicked();
    void registerClicked();

public slots:
    void onLoginClicked();      // 登陆被点击
    void onRegisterClicked();   // 注册被点击

private:
    QPushButton *login;             // 登陆按钮
    QPushButton *register_;         // 注册按钮
};

#endif // HOMEPAGE_H
