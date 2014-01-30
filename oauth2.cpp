#include "oauth2.h"
#include <QDebug>
#include <QApplication>
#include "logindialog.h"
#include <QSettings>
#include <QMessageBox>

OAuth2::OAuth2(QWidget* parent)
{
    m_strEndPoint = "https://accounts.google.com/o/oauth2/auth";
    m_strToken = "https://accounts.google.com/o/oauth2/token";
    m_strScope = "https://www.googleapis.com/auth/tasks";
    m_strClientID = "773994449559-57kdeaogcku39qk3ceqqbebm6fsudc5u.apps.googleusercontent.com";
    m_strRedirectURI = "urn:ietf:wg:oauth:2.0:oob";
   // m_strRedirectURI = "http://localhost";
    m_strResponseType = "code";
    m_strCompanyName = "GTasksManager"; //You company here
    m_strAppName = "test_app_name"; //Your application name here
 //   m_strAccessToken = "4/WgdNZqmRFLVhYtNbsXtKZM6Aldov.sqxe2xtFBdIfPvB8fYmgkJzT2ka0hwI";
    m_pLoginDialog = new LoginDialog(parent);
    m_pParent = parent;
    connect(m_pLoginDialog, SIGNAL(accessTokenObtained()), this, SLOT(accessTokenObtained()));
    connect(m_pLoginDialog, SIGNAL(accessCodeObtained()) , this, SLOT(slotAccessCodeObtained()) );
}

void OAuth2::setScope(const QString& scope)
{
    m_strScope = scope;
}

void OAuth2::setClientID(const QString& clientID)
{
    m_strClientID = clientID;
}

void OAuth2::setRedirectURI(const QString& redirectURI)
{
    m_strRedirectURI = redirectURI;
}

void OAuth2::setCompanyName(const QString& companyName)
{
    m_strCompanyName = companyName;
}

void OAuth2::setAppName(const QString& appName)
{
    m_strAppName = appName;
}



QString OAuth2::loginUrl()
{
    qDebug() <<"QString OAuth2::loginUrl()";
    QString str = QString("%1?client_id=%2&redirect_uri=%3&response_type=%4&scope=%5").arg(m_strEndPoint).arg(m_strClientID).
            arg(m_strRedirectURI).arg(m_strResponseType).arg(m_strScope);
    qDebug() << "Login URL" << str;
    return str;
}

QString OAuth2::tokenUrl()
{
    QString str = QString("%1?code=%2&redirect_uri=%3&client_id=%4&scope=%5").arg(m_strToken).arg(m_pLoginDialog->accessCode()).
            arg(m_strRedirectURI).arg(m_strClientID).arg(m_strScope);
    //qDebug() << "Login URL" << str;
    return str;
}

QString OAuth2::accessToken()
{
    return m_strAccessToken;
}

bool OAuth2::isAuthorized()
{
    return m_strAccessToken != "";
}

void OAuth2::startLogin(bool bForce)
{
    QSettings settings(m_strCompanyName, m_strAppName);
    QString str = settings.value("access_token", "").toString();

    qDebug() << "OAuth2::startLogin, token from Settings" << str;
    if(m_strClientID == "YOUR_CLIENT_ID_HERE" || m_strRedirectURI == "YOUR_REDIRECT_URI_HERE")
    {
        QMessageBox::warning(m_pParent, "Warning",
                             "To work with application you need to register your own application in <b>Google</b>.\n"
                             "Learn more from <a href='http://code.google.com/p/qt-google-tasks/wiki/HowToRegisterYourAppIicationInGoogle'>here</a>");
        return;
    }


    if(str.isEmpty() || bForce)
    {
        qDebug() << "bad if";
        m_pLoginDialog->setLoginUrl(loginUrl());
        m_pLoginDialog->show();
    }
    else
    {
        m_strAccessToken = str;
        emit loginDone();
    }
}

void OAuth2::accessTokenObtained()
{
    QSettings settings(m_strCompanyName, m_strAppName);
    m_strAccessToken = m_pLoginDialog->accessToken();
    settings.setValue("access_token", m_strAccessToken);
    emit loginDone();

}

void OAuth2::slotAccessCodeObtained()
{
    qDebug() << "code obtained and it's : " << m_pLoginDialog->accessCode();
    qDebug() << "token String: " << tokenUrl();
}


