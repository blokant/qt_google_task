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
    void taskListInserted(gTaskList*);
    void taskListNotInserted();
    void taskListDeleted();
    void taskListNotDeleted();
    void taskListUpdated(gTaskList*);
    void taskListRetrieved(gTaskList*);
    //tasks
    void tasksOfListRetrieved(QList<gTask*>*);
    void taskRetrieved(gTask*);
public slots:
    void getTaskLists();
    void getTaskList(QString listId);
    void insertTaskList(QString );
    void deleteTaskList(QString );
    void updateTaskList(gTaskList *);
    //all the process* should be private
    void processTaskListsReply(QNetworkReply*);
    void processinsertTaskListReply(QNetworkReply*);
    void processdeleteTaskListReply(QNetworkReply*);
    void processupdateTaskListReply(QNetworkReply*);
    void processgetTaskListReply(QNetworkReply*);
    //tasks
    void getTasksOfList(QString);
    void getTask(QString listId,QString taskId);
    void processTasksOfListReply(QNetworkReply*);
    void processgetTaskReply(QNetworkReply*);

private slots:
    gTaskList* getTaskListFromByteArray(QByteArray *ba);
    gTaskList* getTaskListFromMap(QVariantMap *m);
    gTask*     getTaskFromByteArray(QByteArray*);
    gTask*     getTaskFromMap(QVariantMap*);
private:
    QString listUrl;
    QString tasksAPIUrl;
    QString accessToken;
    QString listsUrl;
    QNetworkAccessManager *qnam;
};

#endif // GTASKHELPER_H
