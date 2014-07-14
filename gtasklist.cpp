#include "gtasklist.h"
gTaskList::gTaskList(QObject *parent) :
    QObject(parent)
{
}

gTaskList::gTaskList(gTaskList &gtl)
{
    setTitle(gtl.getTitle());
    setId(gtl.getId());
    setSelfLink(gtl.getSelfLink());
    setUpdated(gtl.getUpdated());
}

QByteArray* gTaskList::toJson()
{
    QVariantMap m;
    m.insert("kind" , "tasks#taskList");
    m.insert("id", getId());
    m.insert("title",getTitle());
    m.insert("selflink", getSelfLink());
    QJson::Serializer serializer;
    bool ok;
    QByteArray *ba = new QByteArray();
    *ba = serializer.serialize(m, &ok);
    return ba;
}
