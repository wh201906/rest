#include "settingdialog.h"
#include "ui_settingdialog.h"

SettingDialog::SettingDialog(MySettings* settings, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingDialog)
{
    ui->setupUi(this);

    myset = settings;

    validator = new QIntValidator(0, 99, this);
    editList = new QList<QLineEdit*>();

    editList->append(ui->WhEdit);
    editList->append(ui->WmEdit);
    editList->append(ui->WsEdit);
    editList->append(ui->RhEdit);
    editList->append(ui->RmEdit);
    editList->append(ui->RsEdit);
    for(int i = 0; i < editList->size(); i++)
    {
        (*editList)[i]->setValidator(validator);
    }

    ui->simpleModeBox->setChecked(myset->value("isSimple").toBool());
    ui->forceLockBox->setChecked(myset->value("isForceLock").toBool());
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
    MySettings::Items newItems;

    newItems.insert("isSimple", ui->simpleModeBox->isChecked());
    newItems.insert("isForceLock", ui->forceLockBox->isChecked());
    newItems.insert("Wh", ui->WhEdit->text().toInt());
    newItems.insert("Wm", ui->WmEdit->text().toInt());
    newItems.insert("Ws", ui->WsEdit->text().toInt());
    newItems.insert("Rh", ui->RhEdit->text().toInt());
    newItems.insert("Rm", ui->RmEdit->text().toInt());
    newItems.insert("Rs", ui->RsEdit->text().toInt());
    emit settingChanged(newItems);

    myset->setValues(newItems);

    QDialog::accept();
}
