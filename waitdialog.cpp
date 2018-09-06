#include "waitdialog.h"
#include "ui_waitdialog.h"
#include <QHostAddress>
#include <QVBoxLayout>

WaitDialog::WaitDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::WaitDialog)
{
    ui->setupUi(this);

    this->hostLabel = new QLabel(this);
    this->hostLabel->setVisible(1);

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(this->hostLabel, 0);
    layout->addWidget(ui->label, 1);
    layout->addWidget(ui->pushButton, 2);

    this->setLayout(layout);
}

WaitDialog::~WaitDialog()
{
    delete ui;
}

void WaitDialog::on_pushButton_clicked()
{
    this->reject();
}

void WaitDialog::setHost(const QHostAddress& host, int port)
{
    this->hostLabel->setText(tr("Your Host Address: ") + host.toString() + ":" + QString::number(port));
}
