#ifndef SETTINGDIALOG_H
#define SETTINGDIALOG_H

#include "mysettings.h"

#include <QDialog>
#include <QMessageBox>
#include <QList>
#include <QLineEdit>
#include <QIntValidator>

namespace Ui {
class SettingDialog;
}

class SettingDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SettingDialog(MySettings* settings,QWidget *parent = nullptr);
    ~SettingDialog();


public slots:
    void accept();

private:
    Ui::SettingDialog *ui;
    QList<QLineEdit*>* editList;
    QIntValidator* validator;
    MySettings* myset;

signals:
    void settingChanged(bool isSpl,int Wh,int Wm,int Ws,int Rh, int Rm,int Rs);
};

#endif // SETTINGDIALOG_H
