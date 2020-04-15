#include "settingdialog.h"
#include "ui_settingdialog.h"

SettingDialog::SettingDialog(MySettings* settings,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingDialog)
{
    ui->setupUi(this);

    myset=settings;

    validator=new QIntValidator(0,99,this);
    editList=new QList<QLineEdit*>();

    editList->append(ui->WhEdit);
    editList->append(ui->WmEdit);
    editList->append(ui->WsEdit);
    editList->append(ui->RhEdit);
    editList->append(ui->RmEdit);
    editList->append(ui->RsEdit);
    for(int i=0;i<editList->size();i++)
        (*editList)[i]->setValidator(validator);

    ui->simpleModeBox->setChecked(myset->value("isSpl").toBool());
    ui->WhEdit->setText(QString::number(myset->value("Wh").toInt()));
    ui->WmEdit->setText(QString::number(myset->value("Wm").toInt()));
    ui->WsEdit->setText(QString::number(myset->value("Ws").toInt()));
    ui->RhEdit->setText(QString::number(myset->value("Rh").toInt()));
    ui->RmEdit->setText(QString::number(myset->value("Rm").toInt()));
    ui->RsEdit->setText(QString::number(myset->value("Rs").toInt()));

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
    myset->setValue("isSpl",ui->simpleModeBox->isChecked());
    myset->setValue("Wh",ui->WhEdit->text().toInt());
    myset->setValue("Wm",ui->WmEdit->text().toInt());
    myset->setValue("Ws",ui->WsEdit->text().toInt());
    myset->setValue("Rh",ui->RhEdit->text().toInt());
    myset->setValue("Rm",ui->RmEdit->text().toInt());
    myset->setValue("Rs",ui->RsEdit->text().toInt());
    QDialog::accept();
}
