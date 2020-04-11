#ifndef SETTINGDIALOG_H
#define SETTINGDIALOG_H

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
    explicit SettingDialog(QWidget *parent = nullptr);
    ~SettingDialog();


public slots:
    void accept();

private:
    Ui::SettingDialog *ui;
    QList<QLineEdit*>* editList;
    QIntValidator* validator;

signals:
    void settingChanged(bool isSpl,int Wh,int Wm,int Ws,int Rh, int Rm,int Rs);
};

#endif // SETTINGDIALOG_H
