#include "logindialog.h"
#include "ui_logindialog.h"

#include <QDebug>
#include <QtWebKit/QWebView>
#include <QWebFrame>
#include <QWebElement>
#include <QUrl>
LoginDialog::LoginDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginDialog)
{
    ui->setupUi(this);
    connect(ui->webView, SIGNAL(urlChanged(QUrl)), this, SLOT(urlChanged(QUrl)));
    connect(ui->webView, SIGNAL(loadFinished(bool)) , this, SLOT(loadPageFinished()) );
    m_strTokenAddress = "";
}

LoginDialog::~LoginDialog()
{
    delete ui;
}

void LoginDialog::urlChanged(const QUrl &url)
{
    qDebug() << "void LoginDialog::urlChanged";
    qDebug() << "URL =" << url;
    QString str = url.toString();
    if(str.indexOf("access_token") != -1)
    {
        QStringList query = str.split("#");
        QStringList lst = query[1].split("&");
        for (int i=0; i<lst.count(); i++ )
        {
            QStringList pair = lst[i].split("=");
            if (pair[0] == "access_token")
            {
                m_strAccessToken = pair[1];
                emit accessTokenObtained();
                QDialog::accept();
            }
        }
    }
    if(str.indexOf("code=") != -1)
    {
        qDebug() << "looks like code added";
    }


}

QString LoginDialog::accessToken()
{
    return m_strAccessToken;
}

QString LoginDialog::accessCode()
{
    return m_strAccessCode;
}

void LoginDialog::setLoginUrl(const QString& url)
{
    ui->webView->setUrl(url);
}

bool LoginDialog::loadPageFinished()
{
    QString pageSource = ui->webView->page()->currentFrame()->toHtml();
    //qDebug() << "PageSource: " << pageSource;
    if(pageSource.contains(("Please copy this code")))
    {
        long indexOfCode = pageSource.indexOf("input id=\"code\"");
        long indexOfValueElement = pageSource.indexOf("value=", indexOfCode);
        long indexOfTerminatingQuot = pageSource.indexOf("\"" , indexOfValueElement + 8);
        m_strAccessCode = pageSource.mid(indexOfValueElement + 7, indexOfTerminatingQuot - (indexOfValueElement + 7) );
        emit accessCodeObtained();
    }
    return true;
}
void LoginDialog::requestToken()
{

    //return reply;
}

void LoginDialog::setTokenAddress(QString str)
{
    m_strTokenAddress = str;
}
