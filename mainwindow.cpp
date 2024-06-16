#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    this->stackWidget = new QStackedWidget(this);
    this->homePage = new HomePage(this);
    this->loginPage = new LoginPage(this);
    this->registerPage = new RegisterPage(this);
    this->friendListPage = new FriendListPage(this);
    this->chatPage = new ChattingPage(this);


    this->stackWidget->addWidget(this->homePage);
    this->stackWidget->addWidget(this->loginPage);
    this->stackWidget->addWidget(this->registerPage);
    this->stackWidget->addWidget(this->friendListPage);
    this->stackWidget->addWidget(this->chatPage);
    this->stackWidget->resize(600, 600);

    // 连接信号与槽
    this->handleHomePage();
    this->handleLoginPage();
    this->handleRegisterPage();
    this->handleFriListPage();
    this->handleChattingPage();

}

// 重写关闭客户端的事件
void MainWindow::closeEvent(QCloseEvent *event)
{
    // 向服务器发送信号
    int currentIndex = this->stackWidget->currentIndex();
    string message;
    // 若是homepage、loginpage、registerpage等在主页之前的页面，则直接退出(因为没有账号密码的信息)
    if(currentIndex < 3)
    {
        message = proj::CloseStraightly + "|";
    }
    else
    {
        message = proj::Close + "|" + manager.getMyName() + "|";
    }
    // 退出子线程
    manager.worker->userIsOnline.store(false);
    manager.worker->userNotClosed.store(false);
    manager.sendData(message);
    qDebug() << "关闭客户端";
    QMainWindow::closeEvent(event);
}

// 处理 homepage 页面的事件
void MainWindow::handleHomePage()
{
    // homepage 的登陆按钮被点击
    QObject::connect(this->homePage, &HomePage::loginClicked, [this](){
        // 切换到登陆页面
        this->stackWidget->setCurrentWidget(this->loginPage);
    });

    // hompage 的注册按钮被点击
    QObject::connect(this->homePage, &HomePage::registerClicked, [this](){
        // 切换到注册页面
        this->stackWidget->setCurrentWidget(this->registerPage);
    });
}

// 处理 loginpage 页面的事件
void MainWindow::handleLoginPage()
{
    // 用户点击了返回
    QObject::connect(this->loginPage, &LoginPage::returnClicked, [this](){
        // 切换到 homepage
        this->stackWidget->setCurrentWidget(this->homePage);
    });
    // 用户登陆成功
    QObject::connect(this->loginPage, &LoginPage::loginSuccess, [this](){
        // 显示 mainPage
        this->stackWidget->setCurrentWidget(this->friendListPage);
        // 对数据进行初始化
        manager.initFriList();
        manager.initAppToMeList();
        manager.initAppToOtherList();
        manager.initTmpChatRecord();
        qDebug() << QString(manager.Me->userName.c_str());
        for(auto &i : *manager.friendList)
            qDebug() << i;
        emit this->loginPage->recvAndupdateAllLists();
        // 打开监听线程
        manager.worker->userIsOnline.store(true);
        manager.worker->userNotClosed.store(true);
    });
    // 用户登陆失败
    QObject::connect(this->loginPage, &LoginPage::loginFailed, [this](){
        QMessageBox::critical(this, "登陆失败", "用户名或密码错误, 请检查用户名和密码");
    });
}

// 处理 registerpage 页面的事件
void MainWindow::handleRegisterPage()
{
    // 返回
    QObject::connect(this->registerPage, &RegisterPage::returnClicked, [this](){
        this->stackWidget->setCurrentWidget(this->homePage);
    });
    // 注册成功
    QObject::connect(this->registerPage, &RegisterPage::registerSuccess, [this](){
        // 显示 mainPage
        this->stackWidget->setCurrentWidget(this->friendListPage);
        // 创建本地缓存文件
        // 打开监听线程
        // 打开监听线程
        manager.worker->userIsOnline.store(true);
        manager.worker->userNotClosed.store(true);
    });
    // 注册失败
    QObject::connect(this->registerPage, &RegisterPage::registerFailed, [this](){
        QMessageBox::critical(this, "注册失败", "账号已存在");
    });
}


void MainWindow::handleFriListPage()
{
    // 在登陆后对列表进行更新
    QObject::connect(this->loginPage, &LoginPage::recvAndupdateAllLists, this->friendListPage, &FriendListPage::refreshFriendList);
}


// 函数被执行后就会销毁了，连接只是改变了状态
void MainWindow::handleChattingPage()
{
    QObject::connect(this->friendListPage, &FriendListPage::itemDoubleClickedSignal, [this](QListWidgetItem myFriend){
        this->chatPage->friendName->setText(myFriend.text());
        this->stackWidget->addWidget(this->chatPage);
        this->stackWidget->setCurrentWidget(this->chatPage);
    });

    QObject::connect(this->chatPage, &ChattingPage::sendReturnSignal, [this](){
        this->stackWidget->setCurrentWidget(this->friendListPage);
    });
}

MainWindow::~MainWindow()
{
    qDebug() << "关闭监听线程";
    qDebug() << "主窗口被销毁";
    // delete ui;
}
