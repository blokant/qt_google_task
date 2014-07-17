#include "gtask.h"

gTask::gTask(QObject *parent) :
    QObject(parent)
{
    deleted = false;
    hidden  = false;

}

gTask::gTask(QString taskTitle, QString taskListId)
{
  deleted = hidden = false;
  title = taskTitle;
  taskListId = taskListId;
  due = QDateTime::currentDateTime();
}

QString gTask::toGoogleTimeFormat(QDateTime dt)
{
    QString gt = QString::number(dt.date().year()) + "-" + QString::number(dt.date().month()) + "-" + QString::number(dt.date().day());
    gt += "T" + QString::number(dt.time().hour()) + ":"+ QString::number(dt.time().minute()) +":" + QString::number(dt.time().second() ) + ".000Z";
    return gt;
}

QByteArray *gTask::toJson()
{
    QVariantMap m;
    m.insert("kind" , "tasks#task");
    if(getTitle() != "")
        m.insert("title",getTitle());
    m.insert("due", toGoogleTimeFormat(getDueTo()) );

    QJson::Serializer serializer;
    bool ok;
    QByteArray *ba = new QByteArray();
    *ba = serializer.serialize(m, &ok);
    return ba;
}
