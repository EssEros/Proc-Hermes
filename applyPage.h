#ifndef FRIENDLISTPAGE_H
#define FRIENDLISTPAGE_H

#include <QWidget>
#include <QListWidget>
#include <QString>
#include <QLineEdit>
#include <QStringList>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <mutex>
#include "manager.h"

class ApplyListPage : public QWidget
{
    Q_OBJECT
public:
    explicit ApplyListPage(QWidget *parent = nullptr);

signals:
    void updateFriendList(QString fri);

public slots:
    // 好友申请按钮被点击
//    void onApplyButtonClicked();
    // 同意好友的申请按钮被点击
//    void onAgreeButtonClicked();
    // 删除好友的按钮被点击
    //    void onDeleteButtonClicked();
private:
    void handleAppToMeEvent();
    void handleAppToOthersEvent();

    //    QPushButton *deleteButton;
    QLineEdit *userNameLineEdit;
    QPushButton *applyButton;
    QPushButton *agreeButton;
    QPushButton *rejectButton;
    QLabel *listToMeName;
    QListWidget *applyListToMe;
    QLabel *listToOthersName;
    QListWidget *applyListToOthers;

    QString applyUserName;
    QString clickedUserName;
    std::mutex mtx;
};

#endif // FRIENDLISTPAGE_H
