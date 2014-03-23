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
