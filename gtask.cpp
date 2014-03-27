#include "gtask.h"

gTask::gTask(QObject *parent) :
    QObject(parent)
{
    deleted = false;
    hidden  = false;

}
