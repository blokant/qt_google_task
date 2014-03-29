#include "gtaskhelper.h"

gTaskHelper::gTaskHelper(QObject *parent) :
    QObject(parent)
{
}

QList<gTaskList*> gTaskHelper::getTaskLists()
{
    QList <gTaskList*> tl;
    return tl;
}
