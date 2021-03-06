#include "dialog.h"
#include "ui_dialog.h"
#include <QDebug>
#include <QTextCodec>
Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    conf = new QSettings("MegawarpSoftware", "taskman");
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
    connect(m_pOAuth2, SIGNAL(AccessTokenArrived(QString)) , this, SLOT(slotTokenObtained(QString)) );
    m_pOAuth2->getAccessToken();
}

void Dialog::slotTokenObtained(QString at)
{
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
    //connect(th, SIGNAL(taskListsRetrieved(QList<gTaskList*>*)) , this, SLOT(slotTaskListObtained(QList<gTaskList*>*)) );
    //th->getTaskListId("Default List");
    //connect(th, SIGNAL(taskListIdRetrieved(QString)) , this, SLOT(slotTaskListIdRetrieved(QString)) );
    //th->getTask("MTMyMTA1NjY4MzI1MTM4NjQ5MjY6MDow", "MTMyMTA1NjY4MzI1MTM4NjQ5MjY6MDo4NjkzNTUwNjk");
    //connect(th, SIGNAL(taskRetrieved(gTask*)) , this, SLOT(slotTaskRetrieved(gTask*)) );
    //gTask *gt = new gTask("привет, мир","MTMyMTA1NjY4MzI1MTM4NjQ5MjY6MDow");
    //th->getTaskListByName("Default List");
    //connect(th, SIGNAL(taskListRetrieved(gTaskList*)) , this, SLOT(slotTaskListRetrieved(gTaskList*)) );
    //gt->setDue(QDateTime::currentDateTime());
    //qDebug() << gt->toGoogleTimeFormat(QDateTime::currentDateTime());
    //QByteArray *ba = gt->toJson();
    //qDebug() << *ba;
    //th->insertTask("MTMyMTA1NjY4MzI1MTM4NjQ5MjY6MDow", gt);
    //connect(th, SIGNAL(taskInserted(gTask*)) , this, SLOT(slotTaskInserted(gTask*)) );
    //th->getTasksOfList("MTMyMTA1NjY4MzI1MTM4NjQ5MjY6MDow");
    //connect(th, SIGNAL(tasksOfListRetrieved(QList<gTask*>*)) , this, SLOT(slotTasksOfListObtained(QList<gTask*>*)) );
    //connect(th, SIGNAL(taskListsRetrieved(QList<gTaskList*>*)), this, SLOT(slotTaskListObtained(QList<gTaskList*>*)) );
    //th->getTasksOfListByName("Default List");
    //connect(th, SIGNAL(tasksOfListRetrieved(QList<gTask*>*)), this, SLOT(slotTasksOfListByName(QList<gTask*>*)) );
    //th->insertTaskList("привет");
    //th->deleteTaskListByName("привет");
    //connect(th, SIGNAL(taskListDeleted()) , this , SLOT(slotTaskListDeleted()) );
    //gTask *gt = new gTask("test1");
    //gTask *gt2= new gTask("Проект 2501");
    //th->insertTaskByTaskListTitle("Lab Selling",gt2);
    //connect(th,SIGNAL(taskInserted(gTask*)) , this, SLOT(slotTaskInserted(gTask*)) );
    // th->getTasksOfListByTitle("selfstudy");
    // connect(th, SIGNAL(tasksOfListRetrieved(QList<gTask*>*)) , this, SLOT(slotTasksOfListObtained(QList<gTask*>*)) );
    th->clearTaskListByTitle("Lab Selling");
    connect(th, SIGNAL(taskListCleared()) , this, SLOT(slotTaskListCleared()));
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
    qDebug() << "slotTasksOfListObtained();";
    gTask* gt = NULL;
    for(int i = 0; i < tasks->size(); i++)
    {
   /*     qDebug() << "task " + QString::number(i);
        qDebug() << "title: " << tasks->at(i)->getTitle();
        qDebug() << "due: " << tasks->at(i)->getDueTo().toString();
        qDebug() << "id: " << tasks->at(i)->getId();
        qDebug() << "status: " << tasks->at(i)->getStatus();
        */
        if(tasks->at(i)->getTitle() == "Проект 2501")
        {
            gt = tasks->at(i);
            break;
        }
    }

    qDebug() << "old id: " << gt->getId();
    qDebug() <<"old title: " << gt->getTitle();
    gt->setTitle("Проект 2501");
    QDateTime qdt;
    qdt = gt->getDueTo();
    qdt = qdt.addDays(1);
    gt->setDue(qdt);
    //th->deleteTaskByTaskListId("MTMyMTA1NjY4MzI1MTM4NjQ5MjY6MTE0ODUyNDk6MA", gt->getId());
    th->deleteTaskByTaskListTitle("selfstudy", gt->getId());
    connect(th, SIGNAL(taskDeleted()) , this, SLOT(slotTaskDeleted()) );
   // th->updateTaskByTaskListId("MTMyMTA1NjY4MzI1MTM4NjQ5MjY6MTE0ODUyNDk6MA",gt);
   // th->updateTaskByTaskListTitle("selfstudy", gt);
   // connect(th, SIGNAL(taskUpdated(gTask*)) , this, SLOT(slotTaskUpdated(gTask*)) );
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
    qDebug() << "title: " << gtl->getTitle();
    qDebug() << "id: " << gtl->getId();
}

void Dialog::slotTaskListIdRetrieved(QString tlId)
{
    qDebug() << "Task List Id: " << tlId;
}

void Dialog::slotTasksOfListByName(QList<gTask *> *tasks)
{
    foreach (gTask* gt, *tasks) {
        qDebug() << "title: " << gt->getTitle();
        qDebug() << "id:  " << gt->getId();
    }
}
void Dialog::slotTaskRetrieved(gTask *gt)
{
    qDebug() << "title: " << gt->getTitle();
}

void Dialog::slotTaskInserted(gTask *gt)
{
    qDebug() << "Dialog::slotTaskInserted";
    qDebug() <<"title: " << gt->getTitle();
    qDebug() << "updated: " << gt->getUpdated().toString();
    qDebug() << "due: " <<     gt->getDueTo().toString();
}

void Dialog::slotTaskUpdated(gTask *gt)
{
    qDebug() <<"slotTaskUpdated();";
    qDebug() <<" new title: "<< gt->getTitle();
    qDebug() << "new id: " << gt->getId();
}

void Dialog::slotTaskDeleted()
{
    qDebug() << "slotTaskDeleted();";
}

void Dialog::slotTaskListCleared()
{
    qDebug() << "slotTaskListCleared";
}
