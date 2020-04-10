#include "settingdialog.h"
#include "ui_settingdialog.h"

SettingDialog::SettingDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingDialog)
{
    ui->setupUi(this);
}

SettingDialog::~SettingDialog()
{
    delete ui;
}

void SettingDialog::on_buttonBox_accepted()
{
    emit settingChanged(ui->simpleModeBox->isChecked(),
                        ui->WhEdit->text().toInt(),
                        ui->WmEdit->text().toInt(),
                        ui->WsEdit->text().toInt(),
                        ui->RhEdit->text().toInt(),
                        ui->RmEdit->text().toInt(),
                        ui->RsEdit->text().toInt());
}
