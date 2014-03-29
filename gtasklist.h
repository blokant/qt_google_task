#ifndef GTASKLIST_H
#define GTASKLIST_H

#include <QObject>
#include <QDateTime>
class gTaskList : public QObject
{
    Q_OBJECT
public:
    explicit gTaskList(QObject *parent = 0);
    void setTitle(const QString& newTitle){title = newTitle;}
    QString getTitle(){return title;}
    QString getId(){return id;}
    QString getEtag(){return etag;}
    QString getSelfLing(){return selfLink;}
    QDateTime getUpdated(){return updated;}

signals:

public slots:

private:
    QString id;
    QString title;
    QString etag;
    QString selfLink;
    QDateTime updated;

};

#endif // GTASKLIST_H
