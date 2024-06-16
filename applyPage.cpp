#include "applyPage.h"

ApplyListPage::ApplyListPage(QWidget *parent) : QWidget(parent)
{
    // 初始化
    //    this->applyToMeList = new QStringList();
    //    this->applyToOthersList = new QStringList();

    this->userNameLineEdit = new QLineEdit(this);
    this->applyButton = new QPushButton("申请添加好友", this);
    this->agreeButton = new QPushButton("同意添加好友", this);
    this->rejectButton = new QPushButton("拒绝添加好友", this);
    this->listToMeName = new QLabel("申请列表", this);
    this->listToOthersName = new QLabel("我的申请", this);
    this->applyListToMe = new QListWidget(this);
    this->applyListToOthers = new QListWidget(this);

    this->userNameLineEdit->setPlaceholderText("请输入用户名...");

    QVBoxLayout *layout = new QVBoxLayout(this);

    QHBoxLayout *topLayout = new QHBoxLayout(this);
    topLayout->addWidget(userNameLineEdit, 7);
    topLayout->addWidget(applyButton, 2);

    QHBoxLayout *midLayoutH = new QHBoxLayout(this);
    QVBoxLayout *midLayoutV = new QVBoxLayout(this);
    QVBoxLayout *midLayoutV1 = new QVBoxLayout(this);
    midLayoutH->addWidget(applyListToMe, 7);
    midLayoutV1->addWidget(this->agreeButton);
    midLayoutV1->addWidget(this->rejectButton);
    midLayoutH->addLayout(midLayoutV1, 2);
    midLayoutV->addWidget(listToMeName, 1);
    midLayoutV->addLayout(midLayoutH, 9);

    QHBoxLayout *botLayoutH = new QHBoxLayout(this);
    QVBoxLayout *botLayoutV = new QVBoxLayout(this);
    botLayoutH->addWidget(applyListToOthers, 7);
    botLayoutV->addWidget(listToOthersName, 3);
    botLayoutV->addLayout(botLayoutH, 9);

    layout->addLayout(topLayout, 1);
    layout->addLayout(midLayoutV, 5);
    layout->addLayout(botLayoutV, 5);


    // 处理好友申请
    this->handleAppToMeEvent();
    // 处理我的申请
    this->handleAppToOthersEvent();
}

void ApplyListPage::handleAppToMeEvent()
{
    // 监听到好友申请信息
    QObject::connect(&manager, &Manager::newApplyRecved, [this](QString fri){
        qDebug() << "向我的申请列表中添加 " << fri;
        this->applyListToMe->addItem(fri);
    });

    // 用户选中了列表中的用户
    QObject::connect(this->applyListToMe, &QListWidget::itemClicked, [this](QListWidgetItem *item){
        this->clickedUserName = item->text();
    });

    // 用户点击了同意
    QObject::connect(this->agreeButton, &QPushButton::clicked, [this]()
    {
        qDebug() << "用户点击了同意";
        if(this->clickedUserName.length() > 0)
        {
            qDebug() << "同意了 " << clickedUserName << " 的申请";
            string message = proj::AgreeApplication + "|" + manager.Me->userName + "|" + clickedUserName.toStdString() + "|";
            // 向服务器发送结果
            manager.sendMess(message);
            manager.applyToMeList->removeOne(clickedUserName);
            manager.friendList->push_back(clickedUserName);
            this->clickedUserName = "";
            // 发送更新好友列表
            this->applyListToMe->clear();
            this->applyListToMe->addItems(*manager.applyToMeList);
            // 发送更新好友列表的信号
            emit manager.refreshList();
        }
    });

    // 用户点击了拒绝
    QObject::connect(this->rejectButton, &QPushButton::clicked, this, [=](){
        if(clickedUserName.length() > 0)
        {
            qDebug() << "用户对 " << clickedUserName << " 点击了拒绝";
            QString applyName = this->clickedUserName;
            // 向服务器发送结果
            // proj::RejectMessage|userName|applyName|
            string message = proj::RejectMessage+"|"+manager.Me->userName+"|"+applyName.toStdString()+"|";
            manager.sendMess(message);
            // 将好友账号从列表中删去
            manager.applyToMeList->removeOne(applyName);
            // 更新申请列表
            applyListToMe->clear();
            applyListToMe->addItems(*manager.applyToMeList);
        }
    });
}


void ApplyListPage::handleAppToOthersEvent()
{
    // 结束输入时
    QObject::connect(this->userNameLineEdit, &QLineEdit::returnPressed, [this](){
        this->applyUserName = userNameLineEdit->text();
    });
    QObject::connect(this->userNameLineEdit, &QLineEdit::editingFinished, [this](){
        this->applyUserName = userNameLineEdit->text();
    });

    // 用户点击了确定，发送好友申请
    QObject::connect(this->applyButton, &QPushButton::clicked, [this](){
        if(applyUserName.length() > 0)
        {
            if(manager.friendList->contains(applyUserName))
            {
                qDebug() << "用户已是好友";
                QMessageBox::critical(this, "无法添加", "用户已是好友");
                // 清空输入
                this->applyUserName.clear();
                this->userNameLineEdit->clear();
            }
            else if (manager.Me->userName == applyUserName.toStdString())
            {
                qDebug() << "用户向自己发出好申请，现已拒绝";
                QMessageBox::critical(this, "无法添加", "已一种正常的视角来看，鲁迅无法成为周树人好友");
                // 清空输入
                this->applyUserName.clear();
                this->userNameLineEdit->clear();
            }
            else
            {
                qDebug() << "用户向 " << applyUserName << " 发送了申请";
                string mess = proj::ApplicationMessage + "|" + manager.Me->userName + "|" + applyUserName.toStdString() + "|";
                manager.sendMess(mess);
                // 清空输入
                // applyUserName 要等收到服务器的确认信息后再刷新
                this->userNameLineEdit->clear();
            }
        }
        else
        {
            qDebug() << "用户的输入为空";
            QMessageBox::critical(this, "无法添加", "你无法向Ess申请好友，就像你不能真的成为你自己的好友那样");
        }
    });

    // 服务器确认可以申请
    QObject::connect(&manager, &Manager::applySuccess, this, [=](){
        qDebug() << "在我的申请列表中添加: " << this->applyUserName;
        this->applyListToOthers->addItem(this->applyUserName);
        this->applyUserName.clear();
    });
    // 不能申请
    QObject::connect(&manager, &Manager::applyFailed, this, [=](){
        qDebug() << "无法申请，因为用户不存在";
        QMessageBox::critical(this, "申请失败", "该用户不存在");
        this->applyUserName.clear();
    });
    // 申请被拒绝
    QObject::connect(&manager, &Manager::userAppRejected, this, [=](QString rejName){
        // 更新列表
        QList<QListWidgetItem*> items = applyListToOthers->findItems(rejName, Qt::MatchExactly);
        if(!items.isEmpty())
        {
            auto applyUser = items.begin();
            // 这里只会有一项，因为不会用同名用户，所以直接删除掉即可
            qDebug() << "从申请列表中删除用户 " << (*applyUser)->text() << " , 因为用户的申请被拒绝";
            delete *applyUser;
        }
        else
        {
            qDebug() << "未找到匹配项";
        }
    });

    // 申请被同意
    QObject::connect(&manager, &Manager::userAppAgreed, this, [=](QString friendName){
        // 更新我对他人的申请列表
        qDebug() << "申请被同意，将要更新申请列表";
        QList<QListWidgetItem*> items = this->applyListToOthers->findItems(friendName, Qt::MatchExactly);
        if(!items.isEmpty())
        {
            QList<QListWidgetItem*>::iterator applyUser = items.begin();
            qDebug() << "从申请列表中删除用户 " << (*applyUser)->text() << " , 因为用户的申请被同意";
            delete *applyUser;
        }
        else
        {
            qDebug() << "未找到匹配项";
        }
    });
}


//// 好友申请按钮被点击
//void ApplyListPage::onApplyButtonClicked()
//{

//}

//// 同意好友的申请按钮被点击
//void ApplyListPage::onAgreeButtonClicked()
//{

//}



