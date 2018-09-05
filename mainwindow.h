#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QHostAddress>
#include "chesspiece.h"

class QTcpServer;
class QTcpSocket;

class ChessBoard;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_new_pushButton_clicked();
    void initServer();
    void acceptConnection();
    void connectHost(QHostAddress);
    void on_join_pushButton_clicked();
    void closeServer();

    void receive();
    void send();

    void on_actionNew_game_triggered();

    void on_actionJoin_game_triggered();

    void on_actionQuit_triggered();

    void on_quit_pushButton_clicked();
    void on_surrender_pushButton_clicked();

    void on_actionExport_triggered();

    void win();
    void lose();

    void on_actionLoad_triggered();

protected:
    void resizeEvent(QResizeEvent*);
    void paintEvent(QPaintEvent*);
    void mousePressEvent(QMouseEvent*);

private:
    Ui::MainWindow *ui;
    double radius;
    ChessPiece::Camp camp;

    ChessBoard* board;

    bool running;

    QTcpServer *listenSocket;
    QTcpSocket *RWSocket;

    QHostAddress host;

    QPoint transfrom(QPoint chessBoardLocation);
};

#endif // MAINWINDOW_H