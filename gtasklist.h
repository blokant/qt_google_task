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
    void setId(const QString& newId){ id = newId;}
    void setSelfLink(QString newSelfLink){selfLink = newSelfLink;}
    void setUpdated(QDateTime dt){updated = dt;}
    QString getTitle(){return title;}
    QString getId(){return id;}
    QString getSelfLing(){return selfLink;}
    QDateTime getUpdated(){return updated;}

signals:

public slots:

private:
    QString id;
    QString title;
    QString selfLink;
    QDateTime updated;

};

#endif // GTASKLIST_H
