#include "joindialog.h"
#include "ui_joindialog.h"
#include <QHostAddress>
#include <QSignalMapper>
#include <QMessageBox>
#include <QNetworkInterface>

static QHostAddress getLocalHost()
{
    auto list = QNetworkInterface::allAddresses();
    for (auto address :list) {
        if (address != QHostAddress::LocalHost && address.toIPv4Address()) return address;
    }
    return QHostAddress::LocalHost;
}

JoinDialog::JoinDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::JoinDialog)
{
    ui->setupUi(this);

    QSignalMapper *mapper = new QSignalMapper(this);
    mapper->setMapping(ui->pushButton_0, 0);
    mapper->setMapping(ui->pushButton_1, 1);
    mapper->setMapping(ui->pushButton_2, 2);
    mapper->setMapping(ui->pushButton_3, 3);
    mapper->setMapping(ui->pushButton_4, 4);
    mapper->setMapping(ui->pushButton_5, 5);
    mapper->setMapping(ui->pushButton_6, 6);
    mapper->setMapping(ui->pushButton_7, 7);
    mapper->setMapping(ui->pushButton_8, 8);
    mapper->setMapping(ui->pushButton_9, 9);
    mapper->setMapping(ui->dotPushButton, 10);
    mapper->setMapping(ui->backPushButton, 11);

    connect(ui->pushButton_0, SIGNAL(clicked(bool)), mapper, SLOT(map()));
    connect(ui->pushButton_1, SIGNAL(clicked(bool)), mapper, SLOT(map()));
    connect(ui->pushButton_2, SIGNAL(clicked(bool)), mapper, SLOT(map()));
    connect(ui->pushButton_3, SIGNAL(clicked(bool)), mapper, SLOT(map()));
    connect(ui->pushButton_4, SIGNAL(clicked(bool)), mapper, SLOT(map()));
    connect(ui->pushButton_5, SIGNAL(clicked(bool)), mapper, SLOT(map()));
    connect(ui->pushButton_6, SIGNAL(clicked(bool)), mapper, SLOT(map()));
    connect(ui->pushButton_7, SIGNAL(clicked(bool)), mapper, SLOT(map()));
    connect(ui->pushButton_8, SIGNAL(clicked(bool)), mapper, SLOT(map()));
    connect(ui->pushButton_9, SIGNAL(clicked(bool)), mapper, SLOT(map()));
    connect(ui->dotPushButton, SIGNAL(clicked(bool)), mapper, SLOT(map()));
    connect(ui->backPushButton, SIGNAL(clicked(bool)), mapper, SLOT(map()));

    connect(mapper, SIGNAL(mapped(int)), this, SLOT(input(int)));

    ui->lineEdit->setText(::getLocalHost().toString());
}

void JoinDialog::addText(const QString & st)
{
    ui->lineEdit->setText(ui->lineEdit->text() + st);
}

void JoinDialog::backSpace()
{
    ui->lineEdit->setText(ui->lineEdit->text().chopped(1));
}

void JoinDialog::input(int x)
{
    if (0 <=x && x <= 9) {
        this->addText(QString::number(x));
    } else if (x == 10) this->addText(tr("."));
    else if (x == 11) this->backSpace();
}

JoinDialog::~JoinDialog()
{
    delete ui;
}

void JoinDialog::on_cancle_pushButton_clicked()
{
    this->reject();
}

void JoinDialog::on_OK_pushButton_clicked()
{
    QHostAddress host;
    if (host.setAddress(ui->lineEdit->text())) {
        emit this->joinHost(host);
        this->accept();
    } else {
        QMessageBox::information(this, tr("Hint"), tr("IP is unable to parse"));
        ui->lineEdit->clear();
        return;
    }
}
