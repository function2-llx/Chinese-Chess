#ifndef CHESSPIECE_H
#define CHESSPIECE_H

#include <QPoint>
#include <QPixmap>
#include <QWidget>

class ChessBoard;

class ChessPiece : public QObject
{
    Q_OBJECT

public:
    enum Camp {
        red,
        black
    };

    enum Type {
        shuai,
        shi,
        xiang,
        ma,
        ju,
        pao,
        bing
    };

private:
    bool alive;
    bool selected;

    QPixmap pixmap;

public slots:
    void select();
    void deselect();

protected:
    Camp camp;
    QPoint position;
    QString type;

signals:
    void pieceDied();

public:
    ChessPiece(Camp camp, QPoint position, const QString& type, QObject *parent = 0);

    virtual ~ChessPiece() = 0;

    virtual bool move(ChessBoard* board, QPoint target) = 0;
    void eaten();
    bool isAlive() const;
    Camp getCamp() const;
    QString getType() const;
    QPoint getPosition() const;
    QPixmap getPixmap() const;
    void setPosition(QPoint position);

};

class Shuai : public ChessPiece {
    Q_OBJECT

public:
    Shuai(Camp camp, QPoint position, QObject* parent = 0);
    ~Shuai();

    bool move(ChessBoard *board, QPoint target);
};

class Bing : public ChessPiece {
    Q_OBJECT

public:
    Bing(Camp camp, QPoint position, QObject* parent = 0);
    ~Bing();

    bool move(ChessBoard *board, QPoint target);
};

class Xiang : public ChessPiece {
    Q_OBJECT

public:
    Xiang(Camp camp, QPoint position, QObject* parent = 0);
    ~Xiang();

    bool move(ChessBoard *board, QPoint target);
};


class Shi : public ChessPiece {
    Q_OBJECT

public:
    Shi(Camp camp, QPoint position, QObject* parent = 0);
    ~Shi();

    bool move(ChessBoard *board, QPoint target);
};

class Ju : public ChessPiece {
    Q_OBJECT

public:
    Ju(Camp camp, QPoint position, QObject* parent = 0);
    ~Ju();

    bool move(ChessBoard *board, QPoint target);
};

class Pao : public ChessPiece {
    Q_OBJECT

public:
    Pao(Camp camp, QPoint position, QObject* parent = 0);
    ~Pao();

    bool move(ChessBoard *board, QPoint target);
};

class Ma : public ChessPiece {
    Q_OBJECT

public:
    Ma(Camp camp, QPoint position, QObject* parent = 0);
    ~Ma();

    bool move(ChessBoard *board, QPoint target);
};

#endif // CHESSPIECE_H
