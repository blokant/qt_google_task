#include "dialog.h"
#include <QApplication>
#include <QDebug>
#include "gtask.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Dialog w;
    w.show();
    gTask gt1;
    gt1.setTitle("Test title");
    gt1.setDue(QDateTime::currentDateTime());
    qDebug() << "title: "<< gt1.getTitle() << "  due: " << gt1.getDueTo();
    return a.exec();
}
