#include "dialog.h"
#include "ui_dialog.h"
#include <QDebug>
Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    m_pOAuth2 = new OAuth2(this);
    connect(m_pOAuth2, SIGNAL(loginDone()), this, SLOT(slotLoginDone()));
    QSettings conf("MegawarpSoftware", "taskman");
    //m_pOAuth2->getAccessToken();
    QString v = conf.value("refresh_token").toString();
    if(v.isEmpty())
        m_pOAuth2->startLogin(false);
    else
        slotLoginDone();

}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::slotLoginDone()
{
    QSettings conf("MegawarpSoftware", "taskman");
    qDebug() << "slot login done";
    if(conf.value("refresh_token").toString().isEmpty())
    {
        conf.setValue("access_token" , m_pOAuth2->getAccessToken());
        conf.setValue("refresh_token" ,m_pOAuth2->getRefreshToken());
    }
    qDebug() << conf.value("refresh_token").toString();
    qDebug() << "isvalid: " << m_pOAuth2->isTokenValid();
}
