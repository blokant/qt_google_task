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
    //qDebug() << "processTaskListsReply();";
    QByteArray ba = r->readAll(); // may cause partial answer
   // qDebug() <<"tasklist: " << ba;
    QList<gTaskList*> *gtl = new QList<gTaskList*>();
    QString json = ba;
    //r->deleteLater();
    bool ok;
    JsonObject result = QtJson::parse(json, ok).toMap();
    if(false == ok)
    {
        qDebug() << "troubles with parsing json";
        return;
    }
    //QString at = result["access_token"].toString();
    JsonArray plugins = result["items"].toList();
    gTaskList gt;
    //gt.set
    //qDebug() << result["items"].toList().at(0).toMap()["title"].toString();
    foreach(QVariant plugin, plugins) {
            qDebug() << "  -" << plugin.toMap()["title"].toString();
        }


}
