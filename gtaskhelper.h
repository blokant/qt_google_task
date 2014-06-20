#ifndef GTASKHELPER_H
#define GTASKHELPER_H

#include <QObject>
#include <QEventLoop>
#include <QList>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QByteArray>
#include <QDebug>
#include "qtjson/json.h"
#include "gtasklist.h"
class gTaskHelper : public QObject
{
    Q_OBJECT
public:
    explicit gTaskHelper(QObject *parent = 0);
    gTaskHelper(QNetworkAccessManager*);
    void setAccessToken(QString strAccessToken);
signals:
    void taskListsRetrieved(QList<gTaskList*>);
public slots:
    void getTaskLists();
    void processTaskListsReply(QNetworkReply*);
private:
    QString listUrl;
    QString accessToken;
    QNetworkAccessManager *qnam;
};

#endif // GTASKHELPER_H
