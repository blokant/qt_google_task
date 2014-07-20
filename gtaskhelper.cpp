#include "gtaskhelper.h"
#include <QMessageBox>
#include <QCoreApplication>
#include <QTextCodec>

gTaskHelper::gTaskHelper(QObject *parent) :
    QObject(parent)
{
    listUrl = "https://www.googleapis.com/tasks/v1/users/@me/lists";
    tasksAPIUrl = "https://www.googleapis.com/tasks/v1/";
    currentTask = NULL;
}

gTaskHelper::gTaskHelper(QNetworkAccessManager *q)
{
    listUrl = "https://www.googleapis.com/tasks/v1/users/@me/lists";
    tasksAPIUrl = "https://www.googleapis.com/tasks/v1/";
    qnam = q;
    currentTask = NULL;
}

bool gTaskHelper::checkAccessToken()
{
    if(accessToken.isEmpty())
    {
        qDebug() << "Error: access token is empty";
        return false;
    }
    return true;
}

void gTaskHelper::setAccessToken(QString strAccessToken)
{
    accessToken = strAccessToken;
}

void gTaskHelper::getTaskLists()
{
    if(!checkAccessToken())
        return ;
    QNetworkAccessManager *nwam =  qnam;
    QNetworkRequest *request = new QNetworkRequest(QUrl(listUrl));
    request->setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    QString at = "Bearer " + accessToken;
    request->setRawHeader("Authorization", QByteArray(at.toAscii()));
    nwam->get(*request);
    connect(nwam, SIGNAL(finished(QNetworkReply*)) , this, SLOT(processTaskListsReply(QNetworkReply*)) );
}

void gTaskHelper::getTaskListById(QString listId)
{
    if(!checkAccessToken())
        return ;
    QNetworkAccessManager *nwam =  qnam;
    QNetworkRequest *request = new QNetworkRequest(QUrl(listUrl + "/" + listId));
    request->setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    QString at = "Bearer " + accessToken;
    request->setRawHeader("Authorization", QByteArray(at.toAscii()));
    nwam->get(*request);
    connect(nwam, SIGNAL(finished(QNetworkReply*)) , this, SLOT(processgetTaskListReply(QNetworkReply*)) );
}

void gTaskHelper::getTaskListId(QString listTitle)
{
    getTaskLists();
    searchingListName = listTitle;
    connect(this, SIGNAL(taskListsRetrieved(QList<gTaskList*>*)) , this, SLOT(processgetTaskListId(QList<gTaskList*>*)) );
}

void gTaskHelper::getTaskListByTitle(QString listTitle)
{
    getTaskListId(listTitle);
    connect(this, SIGNAL(taskListIdRetrieved(QString)), this, SLOT(getTaskListById(QString)) );
}

void gTaskHelper::insertTaskList(QString listName)
{
    if(!checkAccessToken())
        return ;
    QNetworkAccessManager *nwam =  qnam;
    QNetworkRequest *request = new QNetworkRequest(QUrl(listUrl));
    request->setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    QString at = "Bearer " + accessToken;
    request->setRawHeader("Authorization", QByteArray(at.toAscii()));
    QString s = "{\n\"title\": \"" + listName + "\" \n}";
    QByteArray data = s.toUtf8();
    nwam->post(*request,data);
    connect(nwam, SIGNAL(finished(QNetworkReply*)) , this, SLOT(processinsertTaskListReply(QNetworkReply*)) );
}

void gTaskHelper::deleteTaskListById(QString listId)
{
    if(!checkAccessToken())
        return ;
    QNetworkAccessManager *nwam =  qnam;
    QNetworkRequest *request = new QNetworkRequest(QUrl(listUrl + "/" + listId));
    request->setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    QString at = "Bearer " + accessToken;
    request->setRawHeader("Authorization", QByteArray(at.toAscii()));
    nwam->deleteResource(*request);
    connect(nwam, SIGNAL(finished(QNetworkReply*)) , this, SLOT(processdeleteTaskListReply(QNetworkReply*)) );
}

void gTaskHelper::deleteTaskListByTitle(QString listTitle)
{
    getTaskListId(listTitle);
    connect(this,SIGNAL(taskListIdRetrieved(QString)) , this, SLOT(deleteTaskListById(QString)) );
}

void gTaskHelper::updateTaskList(gTaskList *gtl)
{
    if(!checkAccessToken())
        return ;
    QNetworkAccessManager *nwam =  qnam;
    QNetworkRequest *request = new QNetworkRequest(QUrl(listUrl + "/" + gtl->getId()));
    request->setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    QString at = "Bearer " + accessToken;
    request->setRawHeader("Authorization", QByteArray(at.toAscii()));
    QByteArray *ba = gtl->toJson();
    nwam->put(*request, *ba);
    delete(ba);
    connect(nwam, SIGNAL(finished(QNetworkReply*)) , this, SLOT(processupdateTaskListReply(QNetworkReply*)) );
}
void gTaskHelper::processTaskListsReply(QNetworkReply *r)
{
    QByteArray ba = r->readAll(); // may cause partial answer
    QList<gTaskList*> *gtl = new QList<gTaskList*>();
    bool ok;
    QJson::Parser parser;

    QVariantMap result = parser.parse (ba, &ok).toMap();
    if (!ok) {
      qFatal("An error occurred during parsing");
      return;
    }
    QList<QVariant> tasklists = result["items"].toList();
    foreach(QVariant tasklist, tasklists) {
            QMap<QString,QVariant> mp = tasklist.toMap();
            gTaskList *gt = getTaskListFromMap(&mp);
            gtl->append(gt);
        }
    r->deleteLater();
    disconnect(qnam, SIGNAL(finished(QNetworkReply*)), this, SLOT(processTaskListsReply(QNetworkReply*)) );
    emit taskListsRetrieved(gtl);
}


void gTaskHelper::getTasksOfListById(QString listId)
{
    if(!checkAccessToken())
        return ;
    QNetworkAccessManager *nwam =  qnam;
    QNetworkRequest *request = new QNetworkRequest(QUrl(tasksAPIUrl + "lists/" + listId + "/tasks/"));
    request->setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    QString at = "Bearer " + accessToken;
    request->setRawHeader("Authorization", QByteArray(at.toAscii()));
    nwam->get(*request);
    connect(nwam, SIGNAL(finished(QNetworkReply*)) , this, SLOT(processTasksOfListReply(QNetworkReply*)) );
    disconnect(this, SIGNAL(taskListIdRetrieved(QString)) , this, SLOT(getTasksOfListById(QString)) );
}

void gTaskHelper::getTasksOfListByTitle(QString taskListTitle)
{
    getTaskListId(taskListTitle);
    connect(this, SIGNAL(taskListIdRetrieved(QString)) , this, SLOT(getTasksOfListById(QString)) );
}

void gTaskHelper::getTask(QString listId, QString taskId)
{
    if(!checkAccessToken())
        return ;
    QNetworkAccessManager *nwam =  qnam;
    QNetworkRequest *request = new QNetworkRequest(QUrl(tasksAPIUrl + "lists/" + listId + "/tasks/" + taskId));
    request->setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    QString at = "Bearer " + accessToken;
    request->setRawHeader("Authorization", QByteArray(at.toAscii()));
    nwam->get(*request);
    connect(nwam, SIGNAL(finished(QNetworkReply*)) , this, SLOT(processgetTaskReply(QNetworkReply*)) );
}

void gTaskHelper::insertTaskByTaskListId(QString listId, gTask *gt)
{
    if(!checkAccessToken())
        return ;
    QNetworkAccessManager *nwam =  qnam;
    QNetworkRequest *request = new QNetworkRequest(QUrl(tasksAPIUrl + "lists/" + listId + "/tasks/"));
    request->setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    QString at = "Bearer " + accessToken;
    request->setRawHeader("Authorization", QByteArray(at.toAscii()));
    QByteArray *ba = gt->toJson();
    nwam->post(*request, *ba);
    delete(ba);
    connect(nwam, SIGNAL(finished(QNetworkReply*)) , this, SLOT(processinsertTaskReply(QNetworkReply*)) );
}

void gTaskHelper::insertTaskByTaskListTitle(QString listTitle, gTask *gt)
{
    getTaskListId(listTitle);
    currentTask = gt;
    connect(this, SIGNAL(taskListIdRetrieved(QString)) , this, SLOT(processinsertTaskByTaskListTitle(QString)) );
}

void gTaskHelper::updateTaskByTaskListId(QString listId, gTask *gt)
{
    if(!checkAccessToken())
        return ;
    QNetworkAccessManager *nwam =  qnam;
    QNetworkRequest *request = new QNetworkRequest(QUrl(tasksAPIUrl + "lists/" + listId + "/tasks/" + gt->getId()));
    request->setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    QString at = "Bearer " + accessToken;
    request->setRawHeader("Authorization", QByteArray(at.toAscii()));
    QByteArray *ba = gt->toJson();
    nwam->put(*request, *ba);
    delete(ba);
    connect(nwam, SIGNAL(finished(QNetworkReply*)) , this, SLOT(processupdateTaskReply(QNetworkReply*)) );
}
void gTaskHelper::updateTaskByTaskListTitle(QString listTitle, gTask *gt)
{
    connect(this,SIGNAL(taskListIdRetrieved(QString)) , this, SLOT(processupdateTaskByTaskListTitle(QString)) );
    getTaskListId(listTitle);
    currentTask = gt;
}

void gTaskHelper::deleteTaskByTaskListId(QString listId, gTask *gt)
{
    if(!checkAccessToken())
        return ;
    QNetworkAccessManager *nwam =  qnam;
    QNetworkRequest *request = new QNetworkRequest(QUrl(tasksAPIUrl + "lists/" + listId + "/tasks/" + gt->getId()));
    request->setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    QString at = "Bearer " + accessToken;
    request->setRawHeader("Authorization", QByteArray(at.toAscii()));
    nwam->deleteResource(*request);
    connect(nwam, SIGNAL(finished(QNetworkReply*)) , this, SLOT(processdeleteTaskByTaskListId(QNetworkReply*)) );
}

void gTaskHelper::processTasksOfListReply(QNetworkReply *r)
{
    QByteArray ba = r->readAll(); // may cause partial answer
    QList<gTask*> *gTaskList = new QList<gTask*>();
    QString json = ba;
    bool ok;
    QJson::Parser parser;

    QVariantMap result = parser.parse (ba, &ok).toMap();
    if (!ok) {
      qFatal("An error occurred during parsing");
      return;
    }
    QList<QVariant> tasklists = result["items"].toList();
    foreach(QVariant gtask, tasklists) {
            QMap<QString,QVariant> mp = gtask.toMap();
            gTask *task = getTaskFromMap(&mp);
            gTaskList->append(task);
        }

    r->deleteLater();
    disconnect(qnam, SIGNAL(finished(QNetworkReply*)), this, SLOT(processTasksOfListReply(QNetworkReply*)) );
    emit tasksOfListRetrieved(gTaskList);
}

void gTaskHelper::processgetTaskReply(QNetworkReply *r)
{
    QByteArray ba = r->readAll(); // may cause partial answer
    gTask* gt = getTaskFromByteArray(&ba);
    if(gt->getTitle().isEmpty() == false)
                emit taskRetrieved(gt);
    r->deleteLater();
    disconnect(qnam, SIGNAL(finished(QNetworkReply*)), this, SLOT(processgetTaskReply(QNetworkReply*)) );
}

void gTaskHelper::processinsertTaskReply(QNetworkReply *r)
{
    QByteArray ba = r->readAll(); // may cause partial answer
    if(ba.isEmpty() )
        return;
    gTask* gt = getTaskFromByteArray(&ba);
    if(gt->getId().isEmpty() == false)
                emit taskInserted(gt);
    r->deleteLater();
    disconnect(qnam, SIGNAL(finished(QNetworkReply*)), this, SLOT(processinsertTaskReply(QNetworkReply*)) );
}

void gTaskHelper::processupdateTaskReply(QNetworkReply *r)
{
    QByteArray ba = r->readAll(); // may cause partial answer
    if(ba.isEmpty() )
        return;
    gTask* gt = getTaskFromByteArray(&ba);
    if(gt->getId().isEmpty() == false)
                emit taskUpdated(gt);
    r->deleteLater();
    disconnect(qnam, SIGNAL(finished(QNetworkReply*)), this, SLOT(processupdateTaskReply(QNetworkReply*)) );
}

void gTaskHelper::processinsertTaskByTaskListTitle(QString taskListId)
{
    insertTaskByTaskListId(taskListId, currentTask);
}

void gTaskHelper::processupdateTaskByTaskListTitle(QString taskListId)
{
    updateTaskByTaskListId(taskListId,currentTask);
}

void gTaskHelper::processdeleteTaskByTaskListId(QNetworkReply *r)
{
    QByteArray ba = r->readAll(); // may cause partial answer
    if(ba.size() == 0)
        emit taskDeleted();
    else
        emit taskNotDeleted();
    r->deleteLater();
    disconnect(qnam, SIGNAL(finished(QNetworkReply*)), this, SLOT(processdeleteTaskByTaskListId(QNetworkReply*)) );
}

void gTaskHelper::processinsertTaskListReply(QNetworkReply *r)
{
    QByteArray ba = r->readAll(); // may cause partial answer
    gTaskList* gt = getTaskListFromByteArray(&ba);
    if(gt->getTitle().isEmpty() == false)
                emit taskListInserted(gt);
    r->deleteLater();
    disconnect(qnam, SIGNAL(finished(QNetworkReply*)), this, SLOT(processinsertTaskListReply(QNetworkReply*)) );
}

void gTaskHelper::processdeleteTaskListReply(QNetworkReply *r)
{
    QByteArray ba = r->readAll(); // may cause partial answer
    if(ba.size() == 0)
        emit taskListDeleted();
    else
        emit taskListNotDeleted();
    r->deleteLater();
    disconnect(qnam, SIGNAL(finished(QNetworkReply*)), this, SLOT(processdeleteTaskListReply(QNetworkReply*)) );
}

void gTaskHelper::processupdateTaskListReply(QNetworkReply *r)
{
    QByteArray ba = r->readAll(); // may cause partial answer
    gTaskList *gt = getTaskListFromByteArray(&ba);
            if(gt->getTitle().isEmpty() == false)
                emit taskListUpdated(gt);

    r->deleteLater();
    disconnect(qnam, SIGNAL(finished(QNetworkReply*)), this, SLOT(processupdateTaskListReply(QNetworkReply*)) );
}

void gTaskHelper::processgetTaskListReply(QNetworkReply *r)
{
    QByteArray ba = r->readAll(); // may cause partial answer
    gTaskList *gt = getTaskListFromByteArray(&ba);
            if(gt->getTitle().isEmpty() == false)
                emit taskListRetrieved(gt);

    r->deleteLater();
    disconnect(qnam, SIGNAL(finished(QNetworkReply*)), this, SLOT(processgetTaskListReply(QNetworkReply*)) );
}

void gTaskHelper::processgetTaskListId(QList<gTaskList*>* tls)
{
    disconnect(this, SIGNAL(taskListsRetrieved(QList<gTaskList*>*)) , this, SLOT(processgetTaskListId(QList<gTaskList*>*)) );
    for(int i = 0 ; i < tls->size(); i++)
    {
        if(tls->at(i)->getTitle() == searchingListName)
        {
          emit taskListIdRetrieved(tls->at(i)->getId());
          break;
        }
    }
    if(tls)
        qDeleteAll(*tls);
}

gTaskList *gTaskHelper::getTaskListFromByteArray(QByteArray *ba) // some kinf of error code?
{
    bool ok;
    QJson::Parser parser;
    QVariantMap result = parser.parse (*ba, &ok).toMap();
    if (!ok) {
      qFatal("An error occurred during parsing");
      return 0;
    }
    QMap<QString,QVariant> mp = result;
    return getTaskListFromMap(&mp);
}

gTaskList *gTaskHelper::getTaskListFromMap(QVariantMap *mp)
{
    gTaskList *gt = new gTaskList();
    gt->setTitle((*mp)["title"].toString());
    gt->setId((*mp)["id"].toString());
    gt->setSelfLink((*mp)["selfLink"].toString());
    QDateTime dt = fromGoogleTimeFormat( (*mp)["updated"].toString() );
    gt->setUpdated(dt);
    return gt;
}

gTask *gTaskHelper::getTaskFromByteArray(QByteArray *ba)
{
    bool ok;
    QJson::Parser parser;
    QVariantMap result = parser.parse (*ba, &ok).toMap();
    if (!ok) {
      qFatal("An error occurred during parsing");
      return 0;
    }
    QMap<QString,QVariant> mp = result;
    return getTaskFromMap(&mp);
}

gTask *gTaskHelper::getTaskFromMap(QVariantMap *mp)
{
    gTask *task = new gTask();
    task->setTitle((*mp)["title"].toString() );
    task->setId((*mp)["id"].toString());
    task->setStatus((*mp)["status"].toString());
    task->setPosition((*mp)["position"].toString());
    task->setEtag((*mp)["etag"].toString());
    task->setSelfLink((*mp)["selfLink"].toString());
    QDateTime dt = fromGoogleTimeFormat( (*mp)["updated"].toString());
    QDateTime due = fromGoogleTimeFormat( (*mp)["due"].toString() );
    task->setDue(due);
    task->setUpdated(dt);
    return task;
}

QDateTime gTaskHelper::fromGoogleTimeFormat(QString dateTimeString)
{
    QStringList sl = dateTimeString.split("T");
    QStringList dateStringList = sl.at(0).split("-");
    QStringList timeStringList = sl.at(1).split(".").at(0).split(":");
    QDate *d = new QDate(dateStringList.at(0).toInt(), dateStringList.at(1).toInt(), dateStringList.at(2).toInt());
    QTime *t = new QTime(timeStringList.at(0).toInt(), timeStringList.at(1).toInt(), timeStringList.at(2).toInt());
    QDateTime *dt = new QDateTime(*d,*t);
    delete(d);
    delete(t);
    return *dt;
}

QString gTaskHelper::toGoogleTimeFormat(QDateTime &dt)
{
    QString gt = QString::number(dt.date().year()) + "-" + QString::number(dt.date().month()) + "-" + QString::number(dt.date().day());
    gt += "T" + QString::number(dt.time().hour()) + ":"+ QString::number(dt.time().minute()) +":" + QString::number(dt.time().second() ) + ".000Z";
    return gt;
}
