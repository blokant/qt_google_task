#include "gtask.h"

gTask::gTask(QObject *parent) :
    QObject(parent)
{
    deleted = false;
    hidden  = false;

}

gTask::gTask(QString taskTitle)
{
  deleted = hidden = false;
  title = taskTitle;
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
    if(getId() != "")
        m.insert("id", getId());
    if(getEtag() != "")
        m.insert("etag", getEtag());
    if(getTitle() != "")
        m.insert("title",getTitle());
    m.insert("due", toGoogleTimeFormat(getDueTo()) );
    if(getUpdated().toString() != "")
        m.insert("updated", toGoogleTimeFormat(getUpdated()));
    if(getSelfLink() != "")
        m.insert("selflink", getSelfLink());
    if(getParentTask() != "")
        m.insert("parent", getParentTask());
    if(getPosition() != "")
        m.insert("position", getPosition());
    if(getNotes() != "")
        m.insert("notes", getNotes());
    if(getStatus() != "")
        m.insert("status", getStatus());
    if(getDueTo().toString() != "")
            m.insert("due", toGoogleTimeFormat(getDueTo())) ;

    QJson::Serializer serializer;
    bool ok;
    QByteArray *ba = new QByteArray();
    *ba = serializer.serialize(m, &ok);
    return ba;
}
