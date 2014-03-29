#include "gtaskhelper.h"

gTaskHelper::gTaskHelper(QObject *parent) :
    QObject(parent)
{
    listUrl = "https://www.googleapis.com/tasks/v1/users/@me/lists";
}

void gTaskHelper::setAccessToken(QString strAccessToken)
{
    accessToken = strAccessToken;
}

QList<gTaskList*> gTaskHelper::getTaskLists()
{
    QList <gTaskList*> tl;
    if(accessToken.isEmpty())
    {
        qDebug() << "Error: access token is empty";
        return tl;
    }



    QNetworkAccessManager *nwam =  new QNetworkAccessManager();
    QNetworkRequest *request = new QNetworkRequest(QUrl(listUrl));
    request->setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    QByteArray data;
    QUrl params;
    params.addQueryItem("access_token",accessToken);


    data.append(params.encodedQuery());
    //data.remove(0,1);
    QNetworkReply *r = nwam->post(*request,data);
    QEventLoop loop;
    connect(nwam, SIGNAL(finished(QNetworkReply*)), &loop, SLOT(quit()) );
    qDebug() << "inside event loop";
    loop.exec();
    if(!r->waitForReadyRead(3000))
    {
        qDebug() << "data is not available";
        return tl;
    }
    r->readAll();
    qDebug() << r->size();
    return tl;
}
