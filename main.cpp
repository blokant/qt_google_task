#include "dialog.h"
#include <QApplication>
#include <QDebug>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Dialog w;
    w.show();

    QSettings conf("MegawarpSoftware", "taskman");
    qDebug() << "from main: " << conf.value("refresh_token").toString();
    return a.exec();
}
