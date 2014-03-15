#include "oauth2.h"
#include <QDebug>
#include <QApplication>
#include "logindialog.h"
#include <QSettings>
#include <QMessageBox>

using QtJson::JsonObject;
using QtJson::JsonArray;

OAuth2::OAuth2(QWidget* parent)
{
    m_strEndPoint = "https://accounts.google.com/o/oauth2/auth";
    m_strTokenAddress = "https://accounts.google.com/o/oauth2/token";
    m_strScope = "https://www.googleapis.com/auth/tasks";
    m_strClientID = "773994449559-57kdeaogcku39qk3ceqqbebm6fsudc5u.apps.googleusercontent.com";
    m_strRedirectURI = "urn:ietf:wg:oauth:2.0:oob";
    m_strClientSecret = "VDtJ21xZOPotTg7o7UJz3MbT";
    m_strResponseType = "code";
    m_strGrantType = "authorization_code";
    m_strCompanyName = "GTasksManager"; //You company here
    m_strAppName = "test_app_name"; //Your application name here
    m_pLoginDialog = new LoginDialog(parent);
    m_pParent = parent;
  //  connect(m_pLoginDialog, SIGNAL(accessTokenObtained()), this, SLOT(accessTokenObtained()));
    connect(m_pLoginDialog, SIGNAL(accessCodeObtained()) , this, SLOT(slotAccessCodeObtained()) );
    conf = new QSettings("MegawarpSoftware", "goauth");
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
    QString str = QString("%1?client_id=%2&redirect_uri=%3&response_type=%4&scope=%5").arg(m_strEndPoint).arg(m_strClientID).
            arg(m_strRedirectURI).arg(m_strResponseType).arg(m_strScope);
    return str;
}

QString OAuth2::tokenUrl()
{
    QString str = QString("%1?code=%2&redirect_uri=%3&client_id=%4&scope=%5&grant_type=%6&client_secret=%7").arg(m_strTokenAddress).arg(m_pLoginDialog->accessCode()).
            arg(m_strRedirectURI).arg(m_strClientID).arg(m_strScope).arg("authorization_code").arg(m_strClientSecret);
    return str;
}

QString OAuth2::getAccessToken()
{
    if(isTokenValid())
    {
        return conf->value("access_token").toString();
    }
    else
    {
        qDebug()  << "QString OAuth2::getAccessToken() : " << "isNotValid() starting to refresh";
        refreshAccessToken();
        return conf->value("access_token").toString();
    }
}

QString OAuth2::getRefreshToken()
{
    return conf->value("refresh_token").toString();
}

bool OAuth2::isAuthorized()
{
    return m_strAccessToken != "";
}

void OAuth2::startLogin(bool bForce)
{
  //  QSettings settings(m_strCompanyName, m_strAppName);
    QString str = conf->value("access_token", "").toString();
    if(m_strClientID == "YOUR_CLIENT_ID_HERE" || m_strRedirectURI == "YOUR_REDIRECT_URI_HERE")
    {
        QMessageBox::warning(m_pParent, "Warning",
                             "To work with application you need to register your own application in <b>Google</b>.\n"
                             "Learn more from <a href='http://code.google.com/p/qt-google-tasks/wiki/HowToRegisterYourAppIicationInGoogle'>here</a>");
        return;
    }

    if(str.isEmpty() || bForce)
    {
        m_pLoginDialog->setLoginUrl(loginUrl());
        m_pLoginDialog->show();
        //m_pLoginDialog->setModal(false);
    }
    else
    {
        m_strAccessToken = str;
        emit loginDone();
    }
}

void OAuth2::accessTokenObtained()
{
   // QSettings settings(m_strCompanyName, m_strAppName);
    m_strAccessToken = m_pLoginDialog->accessToken();
   // settings.setValue("access_token", m_strAccessToken);
    emit loginDone();

}

void OAuth2::slotAccessCodeObtained()
{
    qDebug() << "code obtained and it's : " << m_pLoginDialog->accessCode();
    m_strAccessCode = m_pLoginDialog->accessCode();
   // qDebug() << "token String: " << tokenUrl();
    m_pLoginDialog->setLoginUrl(tokenUrl());
    m_pLoginDialog->show();
    obtainAccessToken();
}

void OAuth2::obtainAccessToken()
{
    m_pLoginDialog->clearWebView();
    qDebug()  << "obtainaccess token";
    if(m_strTokenAddress.isEmpty())
    {
        qDebug() << "token adress should be set by setTokenAdress()";
        return;
    }
    QNetworkAccessManager *nwam =  new QNetworkAccessManager();
    QNetworkRequest *request = new QNetworkRequest(QUrl(m_strTokenAddress));
    request->setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    QByteArray data;
    QUrl params;
    params.addQueryItem("grant_type",m_strGrantType);
    params.addQueryItem("client_id",m_strClientID);
    params.addQueryItem("client_secret",m_strClientSecret);
    params.addQueryItem("code",  m_strAccessCode );
    params.addQueryItem("redirect_uri",m_strRedirectURI);
    params.addQueryItem("scope",m_strScope);
    data.append(params.encodedQuery());
    //data.remove(0,1);
    nwam->post(*request,data);
    connect(nwam, SIGNAL(finished(QNetworkReply*)), this, SLOT(slotProcessPostReply(QNetworkReply*)) );
}

void OAuth2::slotProcessPostReply(QNetworkReply *r) // getting token
{
    qDebug() << "slotProcessPostReply()";
    QString json = r->readAll();
    qDebug() << "reply: " << json;
    if(json.contains("token") == false)
    {
        qDebug() << "json does not contain token info";
        return;
    }
    bool ok;
    JsonObject result = QtJson::parse(json, ok).toMap();
    if (ok)
    {
    //    qDebug() << "token: " << result["access_token"].toString();
     //   qDebug() << "refresh_toke: " << result["refresh_token"].toString();

        QString at = result["access_token"].toString();
        if(at.isEmpty())
        {
            qDebug() << "Error: empty access token" ;
        }
        else
        {
            m_strAccessToken = at;
            conf->setValue("access_token", m_strAccessToken);
            conf->setValue("token_obtained",QDateTime::currentDateTime().toString());
        }
        if(!result["refresh_token"].toString().isEmpty())
        {
            m_strRefreshToken = result["refresh_token"].toString();
            conf->setValue("refresh_token", m_strRefreshToken);
            qDebug() << "getting value: " << m_strRefreshToken;
        }
        else
        {
            qDebug() << "Error: empty refresh token" ;
        }

        //QDateTime dt();
        //dt.currentDateTime().toString()

        //conf->setValue("token",result["access_token"].toString());

        emit loginDone();
        //if(m_pLoginDialog->isActiveWindow())
            m_pLoginDialog->close();
    }
    else
    {
        qDebug() << "can not parse json reply";
    }


}

void OAuth2::refreshAccessToken()
{
    //m_pLoginDialog->clearWebView();

    qDebug() << "refreshAccessToken()";
    if(m_strTokenAddress.isEmpty())
    {
        qDebug() << "token adress should be set by setTokenAdress()";
        return;
    }
    QNetworkAccessManager *nwam =  new QNetworkAccessManager();
    QNetworkRequest *request = new QNetworkRequest(QUrl(m_strTokenAddress));
    request->setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    QByteArray data;
    QUrl params;
    params.addQueryItem("client_secret",m_strClientSecret);
    params.addQueryItem("grant_type","refresh_token");
    params.addQueryItem("refresh_token",  conf->value("refresh_token").toString() );
    params.addQueryItem("client_id",m_strClientID);


  //  qDebug() << "refresh token in params: " << m_strRefreshToken;
    data.append(params.encodedQuery());
    //data.remove(0,1);
    nwam->post(*request,data);
    qDebug() << "data: " << data;
    connect(nwam, SIGNAL(finished(QNetworkReply*)), this, SLOT(slotProcessPostReply(QNetworkReply*)) );
}

bool OAuth2::isTokenValid()
{
    QDateTime tokenObtained = QDateTime::fromString(conf->value("token_obtained").toString());
  /*  qDebug() << "In isValid() : ";
    qDebug() << "obtained: " << tokenObtained.toString();
    qDebug() << "current : " << QDateTime::currentDateTime().toString();
    qDebug() << "dif: " << tokenObtained.secsTo(QDateTime::currentDateTime()) <<  " sec";
   */ if(tokenObtained.secsTo(QDateTime::currentDateTime()) > 3600)
        return false;
    return true;
}
