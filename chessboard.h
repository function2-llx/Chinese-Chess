#ifndef CHESSBOARD_H
#define CHESSBOARD_H

#include <QWidget>
#include <QPixmap>
#include "chesspiece.h"

class ChessPiece;
class QPoint;

class ChessBoard : public QObject
{
    Q_OBJECT

private:
    ChessPiece *piece[10][9], *selectedPiece;
    QPixmap pixmap;

    ChessPiece::Camp activeCamp;

    void findChess(ChessPiece::Camp camp, const QString& type, QString& info) const;

signals:
    void activityEnd();

private slots:

public:
    ChessBoard(QObject* parent = 0);
    ~ChessBoard();

    void setPiece(ChessPiece *piece, int r, int c);
    void setPiece(ChessPiece* piece, QPoint position);
    bool existPiece(int r, int c) const;
    bool existPiece(QPoint position) const;
    ChessPiece* getPiece(int r, int c) const;
    ChessPiece* getPiece(QPoint position) const;

    ChessPiece* getSelectedPiece() const;


    QPixmap getPixmap() const;

    void click(int r, int c);

    bool existWinner() const;
    ChessPiece::Camp getWinner() const;

    ChessPiece::Camp getActiveCamp() const;
    void switchActiveCamp();
    void setActiveCamp(ChessPiece::Camp);

    QString toPlainText() const;
    void load(const QString& info);
};

#endif // CHESSBOARD_H
