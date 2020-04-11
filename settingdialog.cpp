#include "settingdialog.h"
#include "ui_settingdialog.h"

SettingDialog::SettingDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingDialog)
{
    ui->setupUi(this);

    validator=new QIntValidator(0,59,this){

};
    editList=new QList<QLineEdit*>();

    editList->append(ui->WhEdit);
    editList->append(ui->WmEdit);
    editList->append(ui->WsEdit);
    editList->append(ui->RhEdit);
    editList->append(ui->RmEdit);
    editList->append(ui->RsEdit);
    for(int i=0;i<editList->size();i++)
        (*editList)[i]->setValidator(validator);

}

SettingDialog::~SettingDialog()
{
    delete ui;
}



void SettingDialog::accept()
{
    emit settingChanged(ui->simpleModeBox->isChecked(),
                        ui->WhEdit->text().toInt(),
                        ui->WmEdit->text().toInt(),
                        ui->WsEdit->text().toInt(),
                        ui->RhEdit->text().toInt(),
                        ui->RmEdit->text().toInt(),
                        ui->RsEdit->text().toInt());
    QDialog::accept();
}
