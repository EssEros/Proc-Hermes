 #include "mainwindow.h"

#include <QApplication>
#include <QTabWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    qRegisterMetaType<Record>("Record");
    w.setFixedSize(600, 600);

    w.show();
    return a.exec();
}
