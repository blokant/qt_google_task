#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>
#include <QString>
#include <QUrl>

namespace Ui {
    class LoginDialog;
}

class LoginDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LoginDialog(QWidget *parent = 0);
    ~LoginDialog();
    void setLoginUrl(const QString& url);
    QString accessToken();
    QString accessCode();
    void setTokenAddress(QString);
    void clearWebView();

signals:
    void accessTokenObtained();
    void accessCodeObtained();

private slots:
    void urlChanged(const QUrl& url);
    bool loadPageFinished();
    void requestToken();

private:
    Ui::LoginDialog *ui;
    QString m_strAccessToken;
    QString m_strAccessCode;
    QString m_strTokenAddress;
    QString m_StrRefereshToken;
};

#endif // LOGINDIALOG_H
