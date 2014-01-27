#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
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
public slots:
    void slotLoginDone();
};

#endif // DIALOG_H
