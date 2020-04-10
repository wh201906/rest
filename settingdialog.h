#ifndef SETTINGDIALOG_H
#define SETTINGDIALOG_H

#include <QDialog>

namespace Ui {
class SettingDialog;
}

class SettingDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SettingDialog(QWidget *parent = nullptr);
    ~SettingDialog();


private slots:
    void on_buttonBox_accepted();

private:
    Ui::SettingDialog *ui;

signals:
    void settingChanged(bool isSpl,int Wh,int Wm,int Ws,int Rh, int Rm,int Rs);
};

#endif // SETTINGDIALOG_H
