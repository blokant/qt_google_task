#ifndef DIALOG_H
#define DIALOG_H
#include <QSettings>
#include <QDialog>
#include "gtaskhelper.h"
#include <QNetworkAccessManager>
#include "oauth2.h"
namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();

private:
    Ui::Dialog *ui;
    OAuth2 *m_pOAuth2;
    QSettings *conf;
    QNetworkAccessManager *qnam;
    gTaskHelper *th;
public slots:
    void slotLoginDone();
    void slotTokenObtained(QString);
    void slotTaskListObtained(QList<gTaskList*> *lists);
    void slotTasksOfListObtained(QList<gTask*> *tasks);
    void slotTaskListInserted(gTaskList*);
    void slotTaskListDeleted();
    void slotTaskListNotDeleted();
    void slotTaskListUpdated(gTaskList*);
    void slotTaskListRetrieved(gTaskList*);
    void slotTaskListIdRetrieved(QString);
    void slotTasksOfListByName(QList<gTask*> *tasks);
    //tasks
    void slotTaskRetrieved(gTask*);
    void slotTaskInserted(gTask*);
};

#endif // DIALOG_H
