#ifndef GTASK_H
#define GTASK_H

#include <QObject>
#include <QDateTime>
#include <QVariantMap>
class gTask : public QObject
{
    Q_OBJECT
public:
    explicit gTask(QObject *parent = 0);
    gTask(QString taskTitle, QString taskListId);

signals:

public slots:

    QString getId(){return id;}
    QString getEtag(){return etag;}
    QString getTitle(){return title;}
    QString getListId(){return listId;}
    QString getSelfLink(){return selflink;}
    QString getParentTask(){return parentTask;}
    QString getPosition(){return position;}
    QString getNotes(){return notes;}
    QString getStatus(){return status;}
    bool isHidden(){return hidden;}
    bool isDeleted(){return deleted;}
    QDateTime getDueTo(){return due;}
    QDateTime getUpdated(){return updated;}
    void setTitle(QString newTitle){title = newTitle;}
    void setId(QString newId){ id = newId;}
    void setNotes(QString newNotes){notes = newNotes;}
    void setPosition(QString newPosition){position = newPosition;}
    void setParentTask(QString ParentId){parentTask = ParentId;}
    void setStatus(QString newStatus){ status = newStatus;}
    void setDeleted(bool flag){deleted = flag;}
    void setHidden(bool  flag){hidden  = flag;}
    void setDue(QDateTime newDue){due = newDue;}
    void setUpdated(QDateTime newUpdated){updated = newUpdated;}
    QString fromGoogleTimeFormat(QDateTime&);
    QByteArray *toJson();
    //isOverDue?

private:

        QString id;
        QString etag;
        QString title;
        QString selflink;
        QString parentTask;
        QString position;
        QString notes;
        QString status;
        bool deleted;
        bool hidden;
        QDateTime due;
        QDateTime updated;
        QString listId;
};

#endif // GTASK_H
