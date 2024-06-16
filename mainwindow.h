#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>
#include <QStackedWidget>
#include <QMessageBox>
#include <QLineEdit>
#include "loginpage.h"
#include "registerpage.h"
#include "homepage.h"
#include "friendListPage.h"
#include "chattingpage.h"
#include "manager.h"
#include "applyPage.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    // void onConnect();

private:
    // UI
    // Ui::MainWindow *ui;
    // 页面栈，存放页面
    QStackedWidget *stackWidget;
    // 登入页
    HomePage *homePage;
    // 登陆页
    LoginPage *loginPage;
    // 注册页面
    RegisterPage *registerPage;
    // 主页同时也是好友列表页面
    FriendListPage *friendListPage;
    // 聊天页
    ChattingPage *chatPage;

public:
    void handleHomePage();          // 处理 homepage 页面的事件
    void handleLoginPage();         // 处理 loginpage 页面的事件
    void handleRegisterPage();      // 处理 registerpage 页面的事件
    void handleFriListPage();       // 处理 FriendListPage 页面的事件
    void handleChattingPage();      // 处理聊天页面
    void closeEvent(QCloseEvent *event) override;        // 重写关闭客户端的事件
};




#endif // MAINWINDOW_H
