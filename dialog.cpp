#include "dialog.h"
#include "ui_dialog.h"
#include <QDebug>
Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    //m_pOAuth2 = new OAuth2(this);
    conf = new QSettings("MegawarpSoftware", "taskman");
    //m_pOAuth2->getAccessToken();
    //qDebug() << "Befire"
    qnam = new QNetworkAccessManager();
    m_pOAuth2 = new OAuth2(this,qnam);
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
    gTaskHelper *th = new gTaskHelper(qnam);
    th->setAccessToken(at);
    th->getTaskLists();
    connect(th, SIGNAL(taskListsRetrieved(QList<gTaskList*>*)), this, SLOT(slotTaskListObtained(QList<gTaskList*>*)) );
}

void Dialog::slotTaskListObtained(QList<gTaskList *> *lists)
{
    for(int i = 0; i < lists->size(); ++i)
    {
        qDebug() << "---";
        qDebug() << "id: " << lists->at(i)->getId();
        qDebug() << "title: " << lists->at(i)->getTitle();
        qDebug() << "selflink: " << lists->at(i)->getSelfLing();
        qDebug() << "updated: " << lists->at(i)->getUpdated();
        qDebug() << "---";
    }
}
