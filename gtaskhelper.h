#ifndef GTASKHELPER_H
#define GTASKHELPER_H

#include <QObject>
#include <QEventLoop>
#include <QList>
#include <QStringList>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QByteArray>
#include <QDebug>
#include "gtasklist.h"
#include "gtask.h"
#include <qjson/parser.h>
class gTaskHelper : public QObject
{
    Q_OBJECT
public:
    explicit gTaskHelper(QObject *parent = 0);
    gTaskHelper(QNetworkAccessManager*);
    void setAccessToken(QString strAccessToken);
signals:
    void taskListsRetrieved(QList<gTaskList*>*);
    void tasksOfListRetrieved(QList<gTask*>*);
public slots:
    void getTaskLists();
    void insertTaskList(QString listName);
    void getTasksOfList(QString);
    void processTaskListsReply(QNetworkReply*);
    void processTasksOfListReply(QNetworkReply*);
private:
    QString listUrl;
    QString tasksAPIUrl;
    QString accessToken;
    QString listsUrl;
    QNetworkAccessManager *qnam;
};

#endif // GTASKHELPER_H
