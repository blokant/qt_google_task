#ifndef GTASKHELPER_H
#define GTASKHELPER_H

#include <QObject>
#include <QList>
#include "gtasklist.h"
class gTaskHelper : public QObject
{
    Q_OBJECT
public:
    explicit gTaskHelper(QObject *parent = 0);

signals:

public slots:
    QList<gTaskList*> getTaskLists();
};

#endif // GTASKHELPER_H
