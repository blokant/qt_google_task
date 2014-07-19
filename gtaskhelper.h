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
    //taskLists
    void taskListsRetrieved(QList<gTaskList*>*);
    void taskListInserted(gTaskList*);
    void taskListNotInserted();
    void taskListDeleted();
    void taskListNotDeleted();
    void taskListUpdated(gTaskList*);
    void taskListRetrieved(gTaskList*);
    void taskListIdRetrieved(QString);
    //tasks
    void tasksOfListRetrieved(QList<gTask*>*);
    void taskRetrieved(gTask*);
    void taskInserted(gTask*);
    void taskUpdated(gTask*);

public slots:
    void getTaskLists();
    void getTaskListById(QString listId);
    void getTaskListId(QString listTitle);
    void getTaskListByTitle(QString listTitle);
    void insertTaskList(QString );
    void deleteTaskListById(QString listId);
    void deleteTaskListByTitle(QString listTitle);
    void updateTaskList(gTaskList *);
    //all the process* should be private
    void processTaskListsReply(QNetworkReply*);
    void processinsertTaskListReply(QNetworkReply*);
    void processdeleteTaskListReply(QNetworkReply*);
    void processupdateTaskListReply(QNetworkReply*);
    void processgetTaskListReply(QNetworkReply*);
    void processgetTaskListId(QList<gTaskList *> *tls);
    //tasks
    void getTasksOfListById(QString);
    void getTasksOfListByTitle(QString taskListTitle); // what hapns when i have 2 task with the same ttl
    void getTask(QString listId,QString taskId);
    void insertTaskByTaskListId(QString listId, gTask *gt);
    void insertTaskByTaskListTitle(QString listTitle, gTask* gt);
    void updateTaskByTaskListId(QString listId, gTask *gt);
    //void updateTaskByTaskListTitle(QString listTitle, gTask* gt);
    void processTasksOfListReply(QNetworkReply*);
    void processgetTaskReply(QNetworkReply*);
    void processinsertTaskReply(QNetworkReply*);
    void processupdateTaskReply(QNetworkReply* r);
    void processinsertTaskByTaskListTitle(QString taskListId);
    void processupdateTaskByTaskListTitle(QString taskListId);

private slots:
    gTaskList* getTaskListFromByteArray(QByteArray *ba);
    gTaskList* getTaskListFromMap(QVariantMap *m);
    gTask*     getTaskFromByteArray(QByteArray*);
    gTask*     getTaskFromMap(QVariantMap*);
    QDateTime fromGoogleTimeFormat(QString);
    QString   toGoogleTimeFormat(QDateTime&);
private:
    QString listUrl;
    QString tasksAPIUrl;
    QString accessToken;
    QString listsUrl;
    QNetworkAccessManager *qnam;
    QString searchingListName; // for searching Id
    gTask* currentTask;
};

#endif // GTASKHELPER_H
