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
#include "gtasklist.h"
class gTaskHelper : public QObject
{
    Q_OBJECT
public:
    explicit gTaskHelper(QObject *parent = 0);
    void setAccessToken(QString strAccessToken);
signals:

public slots:
    void getTaskLists();
    void processTaskListsReply(QNetworkReply*);
private:
    QString listUrl;
    QString accessToken;
};

#endif // GTASKHELPER_H
