#include "chessboard.h"
#include <QPoint>
#include <QDebug>
#include <QVector>
#include <chesspiece.h>
#include <sstream>

ChessBoard::ChessBoard(QObject *parent) : QObject(parent)
{
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 9; j++) this->piece[i][j] = Q_NULLPTR;
    }

    this->piece[0][0] = new Ju(ChessPiece::black, QPoint(0, 0), this);
    this->piece[0][1] = new Ma(ChessPiece::black, QPoint(0, 1), this);
    this->piece[0][2] = new Xiang(ChessPiece::black, QPoint(0, 2), this);
    this->piece[0][3] = new Shi(ChessPiece::black, QPoint(0, 3), this);
    this->piece[0][4] = new Shuai(ChessPiece::black, QPoint(0, 4), this);
    this->piece[0][5] = new Shi(ChessPiece::black, QPoint(0, 5), this);
    this->piece[0][6] = new Xiang(ChessPiece::black, QPoint(0, 6), this);
    this->piece[0][7] = new Ma(ChessPiece::black, QPoint(0, 7), this);
    this->piece[0][8] = new Ju(ChessPiece::black, QPoint(0, 8), this);

    this->piece[2][1] = new Pao(ChessPiece::black, QPoint(2, 1), this);
    this->piece[2][7] = new Pao(ChessPiece::black, QPoint(2, 7), this);

    this->piece[3][0] = new Bing(ChessPiece::black, QPoint(3, 0), this);
    this->piece[3][2] = new Bing(ChessPiece::black, QPoint(3, 2), this);
    this->piece[3][4] = new Bing(ChessPiece::black, QPoint(3, 4), this);
    this->piece[3][6] = new Bing(ChessPiece::black, QPoint(3, 6), this);
    this->piece[3][8] = new Bing(ChessPiece::black, QPoint(3, 8), this);

    this->piece[6][0] = new Bing(ChessPiece::red, QPoint(6, 0), this);
    this->piece[6][2] = new Bing(ChessPiece::red, QPoint(6, 2), this);
    this->piece[6][4] = new Bing(ChessPiece::red, QPoint(6, 4), this);
    this->piece[6][6] = new Bing(ChessPiece::red, QPoint(6, 6), this);
    this->piece[6][8] = new Bing(ChessPiece::red, QPoint(6, 8), this);

    this->piece[7][1] = new Pao(ChessPiece::red, QPoint(7, 1), this);
    this->piece[7][7] = new Pao(ChessPiece::red, QPoint(7, 7), this);

    this->piece[9][0] = new Ju(ChessPiece::red, QPoint(9, 0), this);
    this->piece[9][1] = new Ma(ChessPiece::red, QPoint(9, 1), this);
    this->piece[9][2] = new Xiang(ChessPiece::red, QPoint(9, 2), this);
    this->piece[9][3] = new Shi(ChessPiece::red, QPoint(9, 3), this);
    this->piece[9][4] = new Shuai(ChessPiece::red, QPoint(9, 4), this);
    this->piece[9][5] = new Shi(ChessPiece::red, QPoint(9, 5), this);
    this->piece[9][6] = new Xiang(ChessPiece::red, QPoint(9, 6), this);
    this->piece[9][7] = new Ma(ChessPiece::red, QPoint(9, 7), this);
    this->piece[9][8] = new Ju(ChessPiece::red, QPoint(9, 8), this);

    this->selectedPiece = Q_NULLPTR;

    this->activeCamp = ChessPiece::red;

    pixmap.load(":/background/background.png");
}

ChessBoard::~ChessBoard() {}

void ChessBoard::setPiece(ChessPiece *piece, int r, int c)
{
    this->setPiece(piece, QPoint(r, c));
}

void ChessBoard::setPiece(ChessPiece *piece, QPoint position)
{
    qDebug() << piece->getType();
    qDebug() << position;
    if (this->existPiece(position)) this->getPiece(position)->eaten();
    this->piece[piece->getPosition().x()][piece->getPosition().y()] = Q_NULLPTR;
    this->piece[position.x()][position.y()] = piece;
    piece->setPosition(position);
}

bool ChessBoard::existPiece(int r, int c) const
{
    return this->piece[r][c] != Q_NULLPTR;
}

bool ChessBoard::existPiece(QPoint position) const
{
    return this->existPiece(position.x(), position.y());
}

ChessPiece* ChessBoard::getPiece(int r, int c) const
{
    return this->piece[r][c];
}

ChessPiece* ChessBoard::getPiece(QPoint position) const
{
    return this->piece[position.x()][position.y()];
}

ChessPiece* ChessBoard::getSelectedPiece() const
{
    return this->selectedPiece;
}

QPixmap ChessBoard::getPixmap() const
{
    return this->pixmap;
}

void ChessBoard::click(int r, int c)
{
    if (r < 0 || c < 0 || r >= 10 || c >= 9) return;
    qDebug() << r << c;
    if (this->selectedPiece != Q_NULLPTR) {
        if (this->selectedPiece == this->piece[r][c]) {
            this->selectedPiece->deselect();
            this->selectedPiece = Q_NULLPTR;
        } else if (this->selectedPiece->move(this, QPoint(r, c))) {
            this->setPiece(this->selectedPiece, r, c);
            this->selectedPiece = Q_NULLPTR;

            this->switchActiveCamp();
            emit this->activityEnd();
        }
    } else {
        if (!this->existPiece(r, c)) return;
        if (this->piece[r][c]->getCamp() != this->activeCamp) return;
        this->selectedPiece = this->piece[r][c];
    }
}

ChessPiece::Camp ChessBoard::getActiveCamp() const
{
    return this->activeCamp;
}

void ChessBoard::setActiveCamp(ChessPiece::Camp activeCamp)
{
    this->activeCamp = activeCamp;
}

void ChessBoard::switchActiveCamp()
{
    if (this->activeCamp == ChessPiece::red) this->activeCamp = ChessPiece::black;
    else this->activeCamp = ChessPiece::red;
}

void ChessBoard::findChess(ChessPiece::Camp camp, const QString &type, QString &info) const
{
    int cnt = 0;
    QVector<QPoint> position;
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 9; j++) {
            if (this->existPiece(i, j)) {
                ChessPiece *piece = this->piece[i][j];
                if (piece->getCamp() == camp && piece->getType() == type) {
                    cnt++;
                    position.push_back(QPoint(j, 9 - i));
                }
            }
        }
    }

    QString cur = QString::number(cnt);
    for (QPoint pos: position) {
        cur += tr(" <") + QString::number(pos.x()) + tr(",") + QString::number(pos.y()) + tr(">");
    }

    info.append(cur + '\n');
}

const static QString type[] = {"Shuai", "Shi", "Xiang", "Ma", "Ju", "Pao", "Bing"};

QString ChessBoard::toPlainText() const
{
    QString info_red, info_black;

    info_red.append(tr("red\n"));
    for (int i = 0; i < 7; i++) this->findChess(ChessPiece::red, type[i], info_red);

    info_black.append(tr("black\n"));
    for (int i = 0; i < 7; i++) this->findChess(ChessPiece::black, type[i], info_black);

    QString info;

    if (this->activeCamp == ChessPiece::red) info.append(info_red + info_black);
    else info.append(info_black + info_red);

    return info;
}

static void read(const char* &s, int &x)
{
    while (*s < '0' || '9' < *s) s++;
    x = *s - '0';
    s++;
    while ('0' <= *s && *s <= '9') x = x * 10 + *s - '0', s++;
}

void ChessBoard::load(const QString &info)
{
    std::string st = info.toStdString();

    bool flag = 0;
    if (info[0] == "r") this->activeCamp = ChessPiece::red;
    else this->activeCamp = ChessPiece::black, flag = 1;

    const char *data = st.c_str();
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 9; j++) if (this->existPiece(i, j)){
            delete this->piece[i][j];
            this->piece[i][j] = Q_NULLPTR;
        }
    }

    this->selectedPiece = Q_NULLPTR;

    for (int k = 0; k < 2; k++) {
        ChessPiece::Camp camp = ChessPiece::Camp(k ^ flag);
        for (int i = 0; i < 7; i++) {
            int cnt = 0;
            ::read(data, cnt);
            ChessPiece::Type type = ChessPiece::Type(i);
            while (cnt--) {
                int r, c;
                ::read(data, c);
                ::read(data, r);
                r = 9 - r;
                switch (type) {
                    case ChessPiece::shuai: {
                        this->piece[r][c] = new Shuai(camp, QPoint(r, c), this);
                        break;
                    }
                    case ChessPiece::shi: {
                        this->piece[r][c] = new Shi(camp, QPoint(r, c), this);
                        break;
                    }
                    case ChessPiece::xiang: {
                        this->piece[r][c] = new Xiang(camp, QPoint(r, c), this);
                        break;
                    }
                    case ChessPiece::ma: {
                        this->piece[r][c] = new Ma(camp, QPoint(r, c), this);
                        break;
                    }
                    case ChessPiece::ju: {
                        this->piece[r][c] = new Ju(camp, QPoint(r, c), this);
                        break;
                    }
                    case ChessPiece::pao: {
                        this->piece[r][c] = new Pao(camp, QPoint(r, c), this);
                        break;
                    }
                    case ChessPiece::bing: {
                        this->piece[r][c] = new Bing(camp, QPoint(r, c), this);
                        break;
                    }
                }
            }
        }
    }
}

bool ChessBoard::existWinner() const
{
//    return this->winnerExist;
    int cnt = 0;
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 9; j++) {
            if (this->existPiece(i, j)) {
                ChessPiece *piece = this->piece[i][j];
                if (piece->getType() == "Shuai") cnt++;
            }
        }
    }
    return cnt != 2;
}

bool ChessBoard::check()
{
    QVector<QPoint> pos_shuai;
    pos_shuai.clear();
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 9; j++) {
            if (this->existPiece(i, j) && this->piece[i][j]->getType() == "Shuai") {
                pos_shuai.push_back(QPoint(i, j));
            }
        }
    }

    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 9; j++) {
            if (this->existPiece(i, j)) {
                ChessPiece *piece = this->piece[i][j];
                for (QPoint position: pos_shuai) {
                    qDebug() << piece->getType();
                    if (piece->move(this, position)) return 1;
                }
            }
        }
    }
    return 0;
}
