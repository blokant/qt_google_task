#include "gtaskhelper.h"
#include <QMessageBox>
#include <QCoreApplication>
gTaskHelper::gTaskHelper(QObject *parent) :
    QObject(parent)
{
    listUrl = "https://www.googleapis.com/tasks/v1/users/@me/lists";
    //listUrl = "http://habrahabr.ru";
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
   // QList <gTaskList*> tl;
    if(accessToken.isEmpty())
    {
        qDebug() << "Error: access token is empty";
        return ;//tl;
    }



    QNetworkAccessManager *nwam =  qnam;
    QNetworkRequest *request = new QNetworkRequest(QUrl(listUrl));
    request->setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    QString at = "Bearer " + accessToken;
    request->setRawHeader("Authorization", QByteArray(at.toAscii()));
    nwam->get(*request);
    connect(nwam, SIGNAL(finished(QNetworkReply*)) , this, SLOT(processTaskListsReply(QNetworkReply*)) );
    //QCoreApplication::exec();
    // execute an event loop to process the request (nearly-synchronous)
   // QEventLoop eventLoop;
    // also dispose the event loop after the reply has arrived
   // connect(nwam, SIGNAL(finished(QNetworkReply *)), &eventLoop, SLOT(quit()));
   // qDebug() << "inside event loop";
   // eventLoop.exec();

    //QMessageBox mb;
    //mb.setWindowTitle("test");
    //connect(r, SIGNAL(finished()), &mb, SLOT(exec()) );

    //mb.exec();
    //connect(nwam, SIGNAL(finished(QNetworkReply*)), &mb, SLOT(exec()) );
}

void gTaskHelper::processTaskListsReply(QNetworkReply *r)
{
    qDebug() << "processTaskListsReply();";
    QByteArray ba = r->readAll();
    qDebug() << ba;
}
