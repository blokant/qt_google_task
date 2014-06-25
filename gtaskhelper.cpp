#include "gtaskhelper.h"
#include <QMessageBox>
#include <QCoreApplication>

using QtJson::JsonObject;
using QtJson::JsonArray;

gTaskHelper::gTaskHelper(QObject *parent) :
    QObject(parent)
{
    listUrl = "https://www.googleapis.com/tasks/v1/users/@me/lists";
}

gTaskHelper::gTaskHelper(QNetworkAccessManager *q)
{
    listUrl = "https://www.googleapis.com/tasks/v1/users/@me/lists";
    qnam = q;
}

void gTaskHelper::setAccessToken(QString strAccessToken)
{
    accessToken = strAccessToken;
}

void gTaskHelper::getTaskLists()
{
    if(accessToken.isEmpty())
    {
        qDebug() << "Error: access token is empty";
        return ;
    }
    QNetworkAccessManager *nwam =  qnam;
    QNetworkRequest *request = new QNetworkRequest(QUrl(listUrl));
    request->setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    QString at = "Bearer " + accessToken;
    request->setRawHeader("Authorization", QByteArray(at.toAscii()));
    nwam->get(*request);
    connect(nwam, SIGNAL(finished(QNetworkReply*)) , this, SLOT(processTaskListsReply(QNetworkReply*)) );
}

void gTaskHelper::processTaskListsReply(QNetworkReply *r)
{
    QByteArray ba = r->readAll(); // may cause partial answer
    QList<gTaskList*> *gtl = new QList<gTaskList*>();
    QString json = ba;
    bool ok;
    JsonObject result = QtJson::parse(json, ok).toMap();
    if(false == ok)
    {
        qDebug() << "troubles with parsing json";
        return;
    }
    JsonArray tasklists = result["items"].toList();
    foreach(QVariant tasklist, tasklists) {
            QMap<QString,QVariant> mp = tasklist.toMap();
            gTaskList *gt = new gTaskList();
            gt->setTitle(mp["title"].toString());
            gt->setId(mp["id"].toString());
            gt->setSelfLink(mp["selfLink"].toString());
            QString dateTimeString = mp["updated"].toString();
            QStringList sl = dateTimeString.split("T");
            QStringList dateStringList = sl.at(0).split("-");
            QStringList timeStringList = sl.at(1).split(".").at(0).split(":");
            QDate *d = new QDate(dateStringList.at(0).toInt(), dateStringList.at(1).toInt(), dateStringList.at(2).toInt());
            QTime *t = new QTime(timeStringList.at(0).toInt(), timeStringList.at(1).toInt(), timeStringList.at(2).toInt());
            QDateTime dt(*d,*t);
            delete(d);
            delete(t);
            gt->setUpdated(dt);
            gtl->append(gt);
        }
    r->deleteLater();
    emit taskListsRetrieved(gtl);
}
