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

    this->host = ::getLocalHost();
    this->running = 0;
}
MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_new_pushButton_clicked()
{
    NewDialog *dialog = new NewDialog(this, this->host);
    dialog->show();
    dialog->setModal(1);

    this->board = new ChessBoard(this);
    this->camp = ChessPiece::red;
    connect(this->board, SIGNAL(activityEnd()), this, SLOT(send()));

    connect(dialog, SIGNAL(accepted()), this, SLOT(initServer()));
}

void MainWindow::initServer()
{
    this->listenSocket = new QTcpServer(this);
    this->listenSocket->listen(QHostAddress::Any, 8888);
    connect(this->listenSocket, SIGNAL(newConnection()), this, SLOT(acceptConnection()));

    WaitDialog dialog;
    dialog.setHost(this->host);
    connect(&dialog, SIGNAL(rejected()), this, SLOT(closeServer()));
    connect(this->listenSocket, SIGNAL(newConnection()), &dialog, SLOT(accept()));

    dialog.exec();
}

void MainWindow::closeServer()
{
    this->listenSocket->close();
}

QPoint MainWindow::transfrom(QPoint chessBoardLocation)
{
    return centralWidget()->pos() +  2 * this->radius * QPoint(chessBoardLocation.y() + 1, chessBoardLocation.x() + 1);
}

void MainWindow::paintEvent(QPaintEvent *)
{
    if (this->running) {
        ui->surrender_pushButton->show();
        ui->groupBox->hide();
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

   //     qDebug() << this->board->toPlainText();

    } else {
        ui->surrender_pushButton->hide();
        ui->groupBox->show();
    }
}

void MainWindow::mousePressEvent(QMouseEvent *e)
{
    if (!this->running) return;
    if (e->button() == Qt::LeftButton && this->camp == this->board->getActiveCamp()) {
        qDebug() << "this camp: " << this->camp << " active camp: " << this->board->getActiveCamp();
        QPoint position = e->pos();
        qDebug() << position;
        double r = this->radius;
        this->board->click((position.y() - centralWidget()->y() - r) / (2 * r), (position.x() - centralWidget()->x() - r) / (2 * r));
        this->update();
    }
}

void MainWindow::sendInfo(const QString &info)
{
    static QByteArray array;
    array.clear();
    array.append(info);
    this->RWSocket->write(array);
}

void MainWindow::acceptConnection()
{
    this->RWSocket = this->listenSocket->nextPendingConnection();
    connect(this->RWSocket, SIGNAL(readyRead()), this, SLOT(receive()));
    qDebug() << "connect success";

    this->sendInfo(this->board->toPlainText());

    this->running = 1;
    listenSocket->close();
    this->update();
}

void MainWindow::on_join_pushButton_clicked()
{
    JoinDialog *dialog = new JoinDialog(this);
    dialog->show();
    dialog->setModal(1);
    this->camp = ChessPiece::black;
    connect(dialog, SIGNAL(joinHost(QHostAddress)), this, SLOT(connectHost(QHostAddress)));
}

void MainWindow::connectHost(QHostAddress host)
{
    this->RWSocket = new QTcpSocket;

    this->RWSocket->connectToHost(host, 8888);

    this->board = new ChessBoard(this);
    connect(this->board, SIGNAL(activityEnd()), this, SLOT(send()));

    this->camp = ChessPiece::black; //无奈之举

    this->running = 1;
    this->board->setActiveCamp(ChessPiece::black);
    connect(this->RWSocket, SIGNAL(readyRead()), this, SLOT(receive()));
}

const static QByteArray surrenderInfo = "surrender";
const static QByteArray timeoutInfo = "time out";

void MainWindow::receive()
{
    qDebug() << "receive during";
    QString info = this->RWSocket->readAll();
    qDebug() << info;
    if (info == ::surrenderInfo || info == ::timeoutInfo) {
        this->win();
    } else {
        this->board->load(info);
        this->update();
        if (this->board->existWinner()) {
            this->lose();
        } else {
            this->board->switchActiveCamp();
        }
        this->update();
    }
}

void MainWindow::send()
{
    this->board->switchActiveCamp();
    QByteArray array;
    array.clear();
    array.append(this->board->toPlainText());
    this->RWSocket->write(array);
    if (this->board->existWinner()) {
        this->win();
    }
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
    QMessageBox::information(this, tr("Hint"), tr("You win"));
    this->running = 0;
}

void MainWindow::lose()
{
    QMessageBox::information(this, tr("Hint"), tr("You lose"));
    this->running = 0;
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

    this->initServer();

    this->update();
}

void MainWindow::on_load_pushButton_clicked()
{
    emit ui->actionLoad->triggered();
}
