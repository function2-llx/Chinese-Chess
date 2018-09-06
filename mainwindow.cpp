#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QtNetwork>
#include <QPainter>
#include "newdialog.h"
#include <QMessageBox>
#include <waitdialog.h>
#include "joindialog.h"
#include <QHostAddress>
#include <QMediaPlayer>
#include "chesspiece.h"
#include <QDebug>
#include "chessboard.h"
#include <QMouseEvent>

static QHostAddress getLocalHost()
{
    auto list = QNetworkInterface::allAddresses();
    for (auto address :list) {
        if (address != QHostAddress::LocalHost && address.toIPv4Address()) return address;
    }
    return QHostAddress::LocalHost;
}


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->setWindowTitle("Chinese chess");

    this->host = ::getLocalHost();
    this->running = 0;
    this->checkPlayer = new QMediaPlayer(this);
    checkPlayer->setMedia(QUrl("qrc:/audio/check.mp3"));
    this->timer = new QTimer(this);
    this->timer->setInterval(1000);
    connect(this->timer, SIGNAL(timeout()), this, SLOT(timeDecrease()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_new_pushButton_clicked()
{
    NewDialog *dialog = new NewDialog(this, this->host);
    dialog->setModal(1);
    dialog->show();

    this->board = new ChessBoard(this);
    this->camp = ChessPiece::red;
    connect(this->board, SIGNAL(activityEnd()), this, SLOT(send()));


    connect(dialog, SIGNAL(accepted()), this, SLOT(initServer()));
}

void MainWindow::initServer()
{
    this->listenSocket = new QTcpServer(this);
    this->listenSocket->listen(QHostAddress::Any, 14514);

    WaitDialog dialog;
    dialog.setHost(this->host);
    connect(&dialog, SIGNAL(rejected()), this, SLOT(closeServer()));
    connect(this->listenSocket, SIGNAL(newConnection()), &dialog, SLOT(accept()));
    connect(&dialog, SIGNAL(accepted()), this, SLOT(acceptConnection()));

    dialog.exec();
}

void MainWindow::closeServer()
{
    delete this->listenSocket;
    delete this->board;
}

QPoint MainWindow::transfrom(QPoint chessBoardLocation)
{
    return centralWidget()->pos() +  2 * this->radius * QPoint(chessBoardLocation.y() + 1, chessBoardLocation.x() + 1);
}

const static QByteArray timeoutInfo = "time out";
const static QByteArray surrenderInfo = "surrender";

void MainWindow::timeDecrease()
{
    if (!this->running) return;
    rest--;
    ui->lcdNumber->display(rest);
    //this->RWSocket->write(::surrenderInfo);

    if (rest == 0 && this->camp == this->board->getActiveCamp()) {
        this->sendInfo(::timeoutInfo);
        this->lose();
    }

}

void MainWindow::paintEvent(QPaintEvent *)
{
    if (this->running) {
        ui->lcdNumber->show();
        if (this->board->getActiveCamp() == this->camp) {
            ui->turn_label->setText("Your turn");
        } else ui->turn_label->setText("Opponent's turn");
        ui->surrender_pushButton->show();
        ui->groupBox->hide();
        ui->turn_label->show();
        QPainter painter(this);
        QPixmap backgroud = this->board->getPixmap().scaled(ui->centralWidget->size(), Qt::KeepAspectRatio);
        painter.drawPixmap(centralWidget()->x(), centralWidget()->y(), backgroud);

        this->radius = backgroud.height() / 22.0;
        double r = this->radius;

        for (int i = 0; i < 10; i++) {
            for (int j = 0; j < 9; j++) {
                if (this->board->existPiece(i, j)) {
                    ChessPiece *piece = this->board->getPiece(i, j);
                    QPixmap icon = piece->getPixmap();
                    painter.drawPixmap(centralWidget()->x() + (2 * j + 1) * r, centralWidget()->y() + (2 * i + 1) * r, 2 * r, 2 * r, icon);
                }
            }
        }

        ChessPiece *selectedPiece = this->board->getSelectedPiece();
        if (selectedPiece != Q_NULLPTR) {
            QPoint leftupCorner = this->transfrom(selectedPiece->getPosition());
            QPen pen;
            pen.setColor(QColor("#FFEC8B"));
            painter.setPen(pen);
            painter.drawEllipse(leftupCorner.x() - r, leftupCorner.y() - r, 2 * r, 2 * r);
        }
    } else {
        ui->turn_label->hide();
        ui->surrender_pushButton->hide();
        ui->lcdNumber->hide();
        ui->groupBox->show();
    }
}

void MainWindow::mousePressEvent(QMouseEvent *e)
{
    if (!this->running) return;
    if (e->button() == Qt::LeftButton && this->camp == this->board->getActiveCamp()) {
        QPoint position = e->pos();
        double r = this->radius;
        this->board->click((position.y() - centralWidget()->y() - r) / (2 * r), (position.x() - centralWidget()->x() - r) / (2 * r));
        this->update();
    }
}


void MainWindow::sendInfo(const QString &info)
{
    QByteArray array;
    array.clear();
    array.append(info);
    this->RWSocket->write(array);
}

const static int time_limit = 180;

void MainWindow::joinSuccess()
{
    connect(this->RWSocket, SIGNAL(readyRead()), this, SLOT(receive()));
    this->board = new ChessBoard(this);
    connect(this->board, SIGNAL(activityEnd()), this, SLOT(send()));


    this->rest = time_limit;
    this->timer->start();
    ui->lcdNumber->display(this->rest);
    this->running = 1;
    this->board->setActiveCamp(ChessPiece::red);

    QMessageBox::information(this, tr("Hint"), tr("Join success"));
\
    this->update();
}

void MainWindow::acceptConnection()
{
    this->RWSocket = this->listenSocket->nextPendingConnection();
    connect(this->RWSocket, SIGNAL(readyRead()), this, SLOT(receive()));

    this->board->setActiveCamp(ChessPiece::red);

    this->running = 1;
    this->rest = time_limit;
    this->timer->start();
    ui->lcdNumber->display(this->rest);
    this->sendInfo(this->board->toPlainText());
    this->update();

    QMessageBox::information(this, tr("Hint"), tr("Connect success"));
    //delete this->listenSocket;
    this->listenSocket->close();
}

void MainWindow::on_join_pushButton_clicked()
{
    JoinDialog *dialog = new JoinDialog(this);
    dialog->setModal(1);
    dialog->show();

    this->camp = ChessPiece::black;
    connect(dialog, SIGNAL(joinHost(QHostAddress,int)), this, SLOT(connectHost(QHostAddress,int)));
}

void MainWindow::connectHost(QHostAddress host, int port)
{
    this->RWSocket = new QTcpSocket;

    WaitDialog dialog;
    connect(&dialog, SIGNAL(rejected()), this, SLOT(giveupJoin()));
    connect(this->RWSocket, SIGNAL(connected()), &dialog, SLOT(accept()));
    connect(&dialog, SIGNAL(accepted()), SLOT(joinSuccess()));
    this->RWSocket->connectToHost(host, port);
    dialog.exec();
}

void MainWindow::giveupJoin()
{
    this->RWSocket->abort();
    delete this->RWSocket;
}

void MainWindow::receive()
{
    QString info = this->RWSocket->readAll();
    if (info == ::surrenderInfo || info == ::timeoutInfo) {
        this->win();
        if (info == ::surrenderInfo) QMessageBox::information(this, tr("Hint"), tr("Oppenent surrendered"));
        if (info == ::timeoutInfo) QMessageBox::information(this, tr("Hint"), tr("Opponent's time out"));

    } else {
        this->board->load(info);
        this->update();
        if (this->board->existWinner()) {
            this->lose();
        } else {
            if (this->board->check()) this->checkPlayer->play();
            this->rest = ::time_limit;
            ui->lcdNumber->display(::time_limit);
        }
    }
}

void MainWindow::send()
{
    QByteArray array;
    array.clear();
    array.append(this->board->toPlainText());
    this->RWSocket->write(array);
    if (this->board->existWinner()) {
        this->win();
    }
    this->rest = ::time_limit;
    this->update();
}

void MainWindow::on_actionNew_game_triggered()
{
    emit ui->new_pushButton->click();
}

void MainWindow::on_actionJoin_game_triggered()
{
    emit ui->join_pushButton->click();
}

void MainWindow::on_actionQuit_triggered()
{
    emit ui->quit_pushButton->click();
}

void MainWindow::on_quit_pushButton_clicked()
{
    qApp->exit();
}

void MainWindow::resizeEvent(QResizeEvent *)
{
    ui->groupBox->move((this->width() - ui->groupBox->width()) / 2, (this->height() - ui->groupBox->height()) / 2);
    ui->surrender_pushButton->move(this->centralWidget()->width() - ui->surrender_pushButton->width(), this->centralWidget()->y());
    ui->lcdNumber->move(this->centralWidget()->width() - ui->lcdNumber->width(), ui->surrender_pushButton->y() + ui->surrender_pushButton->height());
    ui->turn_label->move(this->centralWidget()->width() - ui->turn_label->width(), ui->lcdNumber->y() + ui->lcdNumber->height());
    //ui->groupBox->resize(this->width(), this->height());
}

void MainWindow::on_surrender_pushButton_clicked()
{
    this->RWSocket->write(::surrenderInfo);
    this->lose();
}

void MainWindow::on_actionExport_triggered()
{
    if (!this->running) {
        QMessageBox::information(this, tr("Hint"), tr("no game start"));
        return;
    }
    QString filename = QFileDialog::getSaveFileName(this);
    QFile file(filename);
    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        QMessageBox::information(this, tr("Hint"), tr("export failed"));
        return;
    }
    QTextStream out(&file);

    out << this->board->toPlainText();
}

void MainWindow::win()
{
    this->timer->stop();
    QMessageBox::information(this, tr("Hint"), tr("You win"));
    this->running = 0;
    delete this->RWSocket;
}

void MainWindow::lose()
{
    this->timer->stop();
    QMessageBox::information(this, tr("Hint"), tr("You lose"));
    this->running = 0;
    delete this->RWSocket;
}

void MainWindow::on_actionLoad_triggered()
{
    QString filename = QFileDialog::getOpenFileName(this);
    QFile file(filename);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        QMessageBox::information(this, tr("Hint"), tr("open fail"));
        return;
    }

    QTextStream in(&file);
    QString info;
    info = in.readAll();

    this->board = new ChessBoard(this);
    this->board->load(info);
    this->camp = ChessPiece::red;

    connect(this->board, SIGNAL(activityEnd()), this, SLOT(send()));

    NewDialog *dialog = new NewDialog(this, this->host);
    dialog->setModal(1);
    dialog->show();

    connect(dialog, SIGNAL(accepted()), this, SLOT(initServer()));

    //this->update();
}

void MainWindow::on_load_pushButton_clicked()
{
    emit ui->actionLoad->triggered();
}
