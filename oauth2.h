#ifndef OAUTH2_H
#define OAUTH2_H

#include <QString>
#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QByteArray>
#include <QSettings>
#include <QDateTime>
#include <qjson/parser.h>
//#include "qtjson/json.h"
class LoginDialog;

class OAuth2 : public QObject
{
    Q_OBJECT

public:
    OAuth2(QWidget* parent = 0);
    OAuth2(QWidget *parent, QNetworkAccessManager*);
    void getAccessToken();
    QString getRefreshToken();
    bool isAuthorized();
    void startLogin(bool bForce);

    //Functions to set application's details.
    void setScope(const QString& scope);
    void setClientID(const QString& clientID);
    void setRedirectURI(const QString& redirectURI);
    void setCompanyName(const QString& companyName);
    void setAppName(const QString& appName);
    bool isTokenValid();

signals:
    //Signal that is emitted when login is ended OK.
    void loginDone();
    void AccessTokenArrived(QString at);

private slots:
    void accessTokenObtained();
    void slotAccessCodeObtained();
    void obtainAccessToken();
    void slotProcessPostReply(QNetworkReply*);
    void slotProcessRefreshedToken(QNetworkReply*);
    void refreshAccessToken(); // should be in a separate thread

private:
    QString loginUrl();
    QString tokenUrl();

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
    QSettings *conf;
    QNetworkAccessManager *qnam;
};

#endif // OAUTH2_H
