#ifndef OAUTH2_H
#define OAUTH2_H

#include <QString>
#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QByteArray>
#include <QSettings>
#include "qtjson/json.h"
class LoginDialog;

class OAuth2 : public QObject
{
    Q_OBJECT

public:
    OAuth2(QWidget* parent = 0);
    QString accessToken();
    bool isAuthorized();
    void startLogin(bool bForce);

    //Functions to set application's details.
    void setScope(const QString& scope);
    void setClientID(const QString& clientID);
    void setRedirectURI(const QString& redirectURI);
    void setCompanyName(const QString& companyName);
    void setAppName(const QString& appName);

    QString loginUrl();
    QString tokenUrl();

signals:
    //Signal that is emitted when login is ended OK.
    void loginDone();

private slots:
    void accessTokenObtained();
    void slotAccessCodeObtained();
    void getAccessToken();
    void slotProcessPostReply(QNetworkReply*);

private:
    QString m_strAccessToken;
    QString m_strRefreshToken;
    QString m_strAccessCode;
    QString m_strGrantType;
    QString m_strTokenAddress;
    QString m_strClientSecret;
    QString m_strEndPoint;
    QString m_strScope;
    QString m_strClientID;
    QString m_strRedirectURI;
    QString m_strResponseType;

    QString m_strCompanyName;
    QString m_strAppName;

    LoginDialog* m_pLoginDialog;
    QWidget* m_pParent;
};

#endif // OAUTH2_H
