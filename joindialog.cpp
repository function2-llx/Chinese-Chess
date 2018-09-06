#include "joindialog.h"
#include "ui_joindialog.h"
#include <QHostAddress>
#include <QSignalMapper>
#include <QMessageBox>
#include <QNetworkInterface>
#include <QIntValidator>

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

    ui->IP_lineEdit->setText(::getLocalHost().toString());
    ui->port_lineEdit->setValidator(new QIntValidator(1, 65536, this));

    ui->IP_lineEdit->installEventFilter(this);
    ui->port_lineEdit->installEventFilter(this);

    this->focusLineEdit = ui->IP_lineEdit;

    ui->port_lineEdit->setText(tr("14514"));
}

bool JoinDialog::eventFilter(QObject* target, QEvent *e)
{
    if (e->type() == QEvent::FocusIn) {
        this->focusLineEdit = static_cast<QLineEdit*>(target);
    }
    return 0;
}

void JoinDialog::addText(const QString & st)
{
//    ui->port_lineEdit->setFocus();
    QLineEdit *lineEdit = this->focusLineEdit;
    lineEdit->setText(lineEdit->text() + st);
}

void JoinDialog::backSpace()
{
    QLineEdit *lineEdit = this->focusLineEdit;
    lineEdit->setText(lineEdit->text().chopped(1));
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
    int port = 14514;
    if (host.setAddress(ui->IP_lineEdit->text())) {
        port = ui->port_lineEdit->text().toInt();
        this->accept();
        emit this->joinHost(host, port);
    } else {
        QMessageBox::information(this, tr("Hint"), tr("IP is unable to parse"));
        return;
    }
}
