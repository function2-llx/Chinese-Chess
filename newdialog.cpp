#include "newdialog.h"
#include "ui_newdialog.h"
#include <QIntValidator>
#include <QLineEdit>
#include <QHostAddress>
#include <QNetworkInterface>
#include <QList>



NewDialog::NewDialog(QWidget *parent, QHostAddress host) :
    QDialog(parent),
    ui(new Ui::NewDialog)
{
    ui->setupUi(this);

    ui->lineEdit->setText(host.toString());
    ui->lineEdit_2->setText("14514");
}

NewDialog::~NewDialog()
{
    delete ui;
}

void NewDialog::on_OK_pushButton_clicked()
{

    this->accept();
}

void NewDialog::on_cancle_pushButton_clicked()
{
    this->reject();
}
