#include "dialog.h"
#include "ui_dialog.h"
#include <QDebug>
#include <QTextCodec>
Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    //m_pOAuth2 = new OAuth2(this);
    conf = new QSettings("MegawarpSoftware", "taskman");
    //m_pOAuth2->getAccessToken();
    //qDebug() << "Befire"
    QTextCodec::setCodecForTr(QTextCodec::codecForName("UTF-8"));

    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("UTF-8"));


    qnam = new QNetworkAccessManager();
    m_pOAuth2 = new OAuth2(this,qnam);
    th = new gTaskHelper(qnam);
    connect(m_pOAuth2, SIGNAL(loginDone()), this, SLOT(slotLoginDone()));
    QString v = conf->value("refresh_token").toString();
    if(v.isEmpty())
        m_pOAuth2->startLogin(false);
    else
        slotLoginDone();

}

Dialog::~Dialog()
{
    delete ui;
    delete(qnam);
    delete(m_pOAuth2);
}

void Dialog::slotLoginDone()
{
    //QSettings conf("MegawarpSoftware", "taskman");
  //  qDebug() << "slot login done";
    if(conf->value("refresh_token").toString().isEmpty())
    {
        //conf->setValue("access_token" , m_pOAuth2->getAccessToken());
        //conf->setValue("refresh_token" ,m_pOAuth2->getRefreshToken());
    }
  /*  qDebug() << "in dialog:" << conf->value("refresh_token").toString();
    qDebug() << "isvalid: " << m_pOAuth2->isTokenValid();
    qDebug() << "access_token: " << m_pOAuth2->getAccessToken();
    qDebug() << "m_pOAuth2->getat(): " << m_pOAuth2->getAccessToken();
    qDebug() << "m_pOAuth2->getrt(): " << m_pOAuth2->getRefreshToken();
    */
    //gTaskHelper *th = new gTaskHelper(qnam);
    connect(m_pOAuth2, SIGNAL(AccessTokenArrived(QString)) , this, SLOT(slotTokenObtained(QString)) );
    m_pOAuth2->getAccessToken();
    //th->setAccessToken(m_pOAuth2->getAccessToken());
    //th->getTaskLists();
}

void Dialog::slotTokenObtained(QString at)
{
    //qDebug() << "updated token is: " << at;
    th->setAccessToken(at);
    //th->getTaskList("MTMyMTA1NjY4MzI1MTM4NjQ5MjY6MTE0ODUyNDk6MA");
    //connect(th, SIGNAL(taskListRetrieved(gTaskList*)) , this, SLOT(slotTaskListRetrieved(gTaskList*)) );
    //th->insertTaskList(QString::fromUtf8("Привет") );
    //th->insertTaskList("привет");
    //connect(th,SIGNAL(taskListInserted(gTaskList*)) , this , SLOT(slotTaskListInserted(gTaskList*)) );
    //th->deleteTaskList("MTMyMTA1NjY4MzI1MTM4NjQ5MjY6MTE2NDI3NjY5ODow");
    //connect(th, SIGNAL(taskListDeleted()) , this , SLOT(slotTaskListDeleted()) );
    //connect(th, SIGNAL(taskListNotDeleted()) , this, SLOT(slotTaskListNotDeleted()) );
    //th->insertTaskList("мир21 Innovation_");
    //th->getTaskLists();
    //th->getTasksOfList("MTMyMTA1NjY4MzI1MTM4NjQ5MjY6MDow");
    //connect(th, SIGNAL(tasksOfListRetrieved(QList<gTask*>*)) , this, SLOT(slotTasksOfListObtained(QList<gTask*>*)) );
    //connect(th, SIGNAL(taskListsRetrieved(QList<gTaskList*>*)), this, SLOT(slotTaskListObtained(QList<gTaskList*>*)) );
}

void Dialog::slotTaskListObtained(QList<gTaskList *> *lists)
{
    for(int i = 0; i < lists->size(); ++i)
    {
        qDebug() << "---";
        qDebug() << "id: " << lists->at(i)->getId();
        qDebug() << "title: " << lists->at(i)->getTitle();
        qDebug() << "selflink: " << lists->at(i)->getSelfLink();
        qDebug() << "updated: " << lists->at(i)->getUpdated();
        qDebug() << "---";
    }
}

void Dialog::slotTasksOfListObtained(QList<gTask *> *tasks)
{
    for(int i = 0; i < tasks->size(); i++)
    {
        qDebug() << "task " + QString::number(i);
        qDebug() << "title: " << tasks->at(i)->getTitle();
        qDebug() << "due: " << tasks->at(i)->getDueTo().toString();
        qDebug() << "id: " << tasks->at(i)->getId();
        qDebug() << "status: " << tasks->at(i)->getStatus();
    }
}

void Dialog::slotTaskListInserted(gTaskList *gtl)
{
    qDebug() << "slotTaskListInserted(gtl);";
    qDebug() << "title: " << gtl->getTitle();
    qDebug() << "id: " << gtl->getId();
    //gtl->setTitle("новое название");
    //th->updateTaskList(gtl);
    //connect(th, SIGNAL(taskListUpdated(gTaskList*)) , this, SLOT(slotTaskListUpdated(gTaskList*)) );
}

void Dialog::slotTaskListDeleted()
{
    qDebug() << "slotTaskListDeleted();";
}

void Dialog::slotTaskListNotDeleted()
{
    qDebug() << "slotTaskListNotDeleted();";
}

void Dialog::slotTaskListUpdated(gTaskList *gtl)
{
    qDebug() <<"slotTaskListUpdated();";
    //make something like getGTLFromReply();
    qDebug() << "new title: " << gtl->getTitle();
}

void Dialog::slotTaskListRetrieved(gTaskList *gtl)
{
    qDebug() << "title:" << gtl->getTitle();
}
