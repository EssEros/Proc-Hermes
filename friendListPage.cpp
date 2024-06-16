#include "friendListPage.h"


FriendListPage::FriendListPage(QWidget *parent)
{
    this->friendList = new QStringList;
    QVBoxLayout *layout = new QVBoxLayout(this);
    stackWidget = new QStackedWidget(this);
    this->myFriendsList = new QListWidget(this);
    this->applyListPage = new ApplyListPage(this);
    this->stackWidget->addWidget(myFriendsList);
    this->stackWidget->addWidget(applyListPage);

    QHBoxLayout *buttonsLayout = new QHBoxLayout(this);
    this->switchMyFriList = new QPushButton("好友列表", this);
    this->switchApplyPage = new QPushButton("申请列表", this);
    buttonsLayout->addWidget(switchMyFriList);
    buttonsLayout->addWidget(switchApplyPage);

    layout->addWidget(stackWidget, 9);
    layout->addLayout(buttonsLayout, 1);
    // 将信息添加进去
    this->myFriendsList->resize(600, 600);

    // 聊天对应的信号与槽
    QObject::connect(this->myFriendsList, &QListWidget::itemDoubleClicked, this, &FriendListPage::onSomebodyClicked);
    // 点击按钮跳转到好友列表
    QObject::connect(switchMyFriList, &QPushButton::clicked, this, [=](){
        if(stackWidget->currentWidget() != myFriendsList)
        {
            stackWidget->setCurrentWidget(myFriendsList);
        }
    });
    // 点击按钮跳转到申请列表
    QObject::connect(switchApplyPage, &QPushButton::clicked, this, [=](){
        if(stackWidget->currentWidget() != applyListPage)
        {
            stackWidget->setCurrentWidget(applyListPage);
        }
    });
    // 好友申请被同意后就更新好友列表
    QObject::connect(&manager, &Manager::userAppAgreed, this, [=](QString friendName){
        qDebug() << "向好友列表中添加好友: " << friendName;
        this->myFriendsList->addItem(friendName);
    });
    // 接收到向表中添加好友的信号
    QObject::connect(&manager, &Manager::refreshList, this, [=](){
        this->myFriendsList->clear();
        this->myFriendsList->addItems(*manager.friendList);
    });
}


FriendListPage::~FriendListPage()
{
    delete friendList;
    delete myFriendsList;
}

void FriendListPage::refreshFriendList()
{
    this->myFriendsList->clear();
    this->myFriendsList->addItems(*manager.friendList);
}

void FriendListPage::onSomebodyClicked(QListWidgetItem *myFriend)
{
    qDebug() << QString(manager.getMyName().c_str()) << " 选择与 " << myFriend->text() << " 聊天";
    emit this->itemDoubleClickedSignal(*myFriend);
}
