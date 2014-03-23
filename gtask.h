#ifndef GTASK_H
#define GTASK_H

#include <QObject>
#include <QDateTime>
class gTask : public QObject
{
    Q_OBJECT
public:
    explicit gTask(QObject *parent = 0);

signals:

public slots:

    QString getId(){return id;}
    QString getEtag(){return etag;}
    QString getTitle(){return title;}
    QString getSelfLink(){return selflink;}
    QString getParent(){return parent;}
    QString getPosition(){return position;}
    QString getNotes(){return notes;}
    QString getStatus(){return status;}
    bool isHidden(){return hidden;}
    bool isDeleted(){return deleted;}
    QDateTime getDueTo(){return due;}
    QDateTime getUpdated(){return updated;}

private:

        QString id;
        QString etag;
        QString title;
        QString selflink;
        QString parent;
        QString position;
        QString notes;
        QString status;
        bool deleted;
        bool hidden;
        QDateTime due;
        QDateTime updated;

};

#endif // GTASK_H
