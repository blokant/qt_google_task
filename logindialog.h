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

signals:
    void accessTokenObtained();
    void accessCodeObtained();

private slots:
    void urlChanged(const QUrl& url);
    bool loadPageFinished();

private:
    Ui::LoginDialog *ui;
    QString m_strAccessToken;
    QString m_strAccessCode;
};

#endif // LOGINDIALOG_H
