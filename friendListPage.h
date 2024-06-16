#ifndef CHATPAGE_H
#define CHATPAGE_H

#include <QMainWindow>
#include <QWidget>
#include <QListWidget>
#include <QString>
#include <QStringList>
#include <QPushButton>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QStackedWidget>
#include "manager.h"
#include "applyPage.h"

class FriendListPage : public QWidget
{
    Q_OBJECT
public:
    explicit FriendListPage(QWidget *parent = nullptr);
    ~FriendListPage();
signals:
//    void chatWith(QListWidgetItem somebody);
    void itemDoubleClickedSignal(QListWidgetItem myFriend);

public slots:
    void refreshFriendList();
    void onSomebodyClicked(QListWidgetItem *myFriend);

private:
    QStackedWidget *stackWidget;
    QStringList *friendList;
    QListWidget *myFriendsList;
    QPushButton *switchMyFriList;
    QPushButton *switchApplyPage;
    // 申请列表页
    ApplyListPage *applyListPage;
};

#endif // CHATPAGE_H
