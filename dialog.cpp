#include "dialog.h"
#include "ui_dialog.h"
#include <QDebug>
Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    OAuth2 *m_pOAuth2 = new OAuth2(this);
    connect(m_pOAuth2, SIGNAL(loginDone()), this, SLOT(slotLoginDone()));
    m_pOAuth2->startLogin(true);

}

Dialog::~Dialog()
{
    delete ui;
}

void Dialog::slotLoginDone()
{
    qDebug() << "slot login done";
}
