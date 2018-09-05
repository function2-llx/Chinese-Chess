#include "chesspiece.h"
#include "chessboard.h"
#include <QDebug>
#include <QLabel>

ChessPiece::ChessPiece(Camp camp, QPoint position, const QString& type, QObject* parent) : QObject(parent), camp(camp), position(position), type(type)
{
    this->alive = 1;
    this->selected = 0;
    QString iconFileName = tr(":/icons/") + type + "_" + tr(camp == red ? "red" : "black") + tr(".png");
    this->pixmap.load(iconFileName);
}

ChessPiece::~ChessPiece() {}

void ChessPiece::eaten()
{
    this->alive = 0;
    emit this->pieceDied();
}

bool ChessPiece::isAlive() const
{
    return this->alive;
}

ChessPiece::Camp ChessPiece::getCamp() const
{
    return this->camp;
}

QString ChessPiece::getType() const
{
    return this->type;
}

void ChessPiece::select()
{
    this->selected = 1;
}

void ChessPiece::deselect()
{
    this->selected = 0;
}

QPoint ChessPiece::getPosition() const
{
    return this->position;
}

QPixmap ChessPiece::getPixmap() const
{
    return this->pixmap;
}

void ChessPiece::setPosition(QPoint position)
{
    this->position = position;
}

Shuai::Shuai(Camp camp, QPoint position, QObject* parent) : ChessPiece(camp, position, "Shuai", parent) {}

Shuai::~Shuai() {}

bool Shuai::move(ChessBoard *board, QPoint target)
{
    if (target == this->position) return 0;
    ChessPiece *targetPiece = board->getPiece(target);
    if (targetPiece != Q_NULLPTR && targetPiece->getCamp() == this->camp) return 0;

    if (target.y() == this->position.y() && targetPiece != Q_NULLPTR && targetPiece->getType() == "Shuai") {    //对将
        for (int i = 1; i <= 8; i++) {
            if (board->getPiece(QPoint(i, target.y())) != Q_NULLPTR) {
                return 0;
            }
        }
    } else {
        if (abs(target.x() - this->position.x()) + abs(target.y() - position.y()) != 1) return 0;
        if (this->camp == black) {
            if (abs(target.x() - 1) > 1 || abs(target.y() - 4) > 1) return 0;
        } else {
            if (abs(target.x() - 8) > 1 || abs(target.y() - 4) > 1) return 0;
        }
    }

    return 1;
}

Bing::Bing(Camp camp, QPoint position, QObject *parent) : ChessPiece(camp, position, "Bing", parent) {}

Bing::~Bing() {}

bool Bing::move(ChessBoard *board, QPoint target)
{
    if (target == this->position) return 0;
    ChessPiece *targetPiece = board->getPiece(target);
    if (targetPiece != Q_NULLPTR && targetPiece->getCamp() == this->camp) return 0;

    if (abs(target.x() - this->position.x()) > 1 || abs(target.y() - this->position.y()) > 1) return 0;

    if (this->camp == black) {
        if (target.x() < position.x()) return 0;
        if (target.x() <= 4 && target.y() != position.y()) return 0;
    } else {
        if (target.x() > position.x()) return 0;
        if (target.x() >= 5 && target.y() != position.y()) return 0;
    }

    return 1;
}

Xiang::Xiang(Camp camp, QPoint position, QObject *parent) : ChessPiece(camp, position, "Xiang", parent) {}

Xiang::~Xiang() {}

bool Xiang::move(ChessBoard *board, QPoint target)
{
    if (target == this->position) return 0;
    ChessPiece *targetPiece = board->getPiece(target);
    if (targetPiece != Q_NULLPTR && targetPiece->getCamp() == this->camp) return 0;

    if (abs(target.x() - this->position.x()) != 2 || abs(target.y() - position.y()) != 2) return 0;

    if (this->camp == black) {
        if (target.x() >= 5) return 0;
    } else {
        if (target.x() <= 4) return 0;
    }

    QPoint mid = (this->position + target) / 2;
    if (board->existPiece(mid)) return 0;

    return 1;
}

Shi::Shi(Camp camp, QPoint position, QObject *parent) : ChessPiece(camp, position, "Shi", parent) {}

Shi::~Shi() {}

bool Shi::move(ChessBoard *board, QPoint target)
{
    if (target == this->position) return 0;
    ChessPiece *targetPiece = board->getPiece(target);
    if (targetPiece != Q_NULLPTR && targetPiece->getCamp() == this->camp) return 0;

    if (abs(target.x() - this->position.x()) != 1 || abs(target.y() - this->position.y()) != 1) return 0;

    if (this->camp == black) {
        if (abs(target.x() - 1) > 1 || abs(target.y() - 4) > 1) return 0;
    } else {
        if (abs(target.x() - 8) > 1 || abs(target.y() - 4) > 1) return 0;
    }

    return 1;
}

Ju::Ju(Camp camp, QPoint position, QObject *parent) : ChessPiece(camp, position, "Ju", parent) {}

Ju::~Ju() {}

bool Ju::move(ChessBoard *board, QPoint target)
{
    if (target == this->position) return 0;
    ChessPiece *targetPiece = board->getPiece(target);
    if (targetPiece != Q_NULLPTR && targetPiece->getCamp() == this->camp) return 0;

    if (target.x() != position.x() && target.y() != position.y()) return 0;

    if (target.x() == position.x()) {
        int a = target.y(), b = position.y();
        if (a > b) std::swap(a, b);
        for (int i = a + 1; i < b; i++) {
            if (board->existPiece(target.x(), i)) return 0;
        }
    } else {
        int a = target.x(), b = position.x();
        if (a > b) std::swap(a, b);
        for (int i = a + 1; i < b; i++) {
            if (board->existPiece(i, target.y())) return 0;
        }
    }

    return 1;
}

Pao::Pao(Camp camp, QPoint position, QObject *parent) : ChessPiece(camp, position, "Pao", parent) {}

Pao::~Pao() {}

bool Pao::move(ChessBoard *board, QPoint target)
{
    if (target == this->position) return 0;
    ChessPiece *targetPiece = board->getPiece(target);
    if (targetPiece != Q_NULLPTR && targetPiece->getCamp() == this->camp) return 0;

    if (targetPiece != Q_NULLPTR) {
        int cnt = 0;
        if (target.x() == position.x()) {
            int a = target.y(), b = position.y();
            if (a > b) std::swap(a, b);
            for (int i = a + 1; i < b; i++) {
                if (board->existPiece(target.x(), i)) cnt++;
            }
        } else {
            int a = target.x(), b = position.x();
            if (a > b) std::swap(a, b);
            for (int i = a + 1; i < b; i++) {
                if (board->existPiece(i, target.y())) cnt++;
            }
        }
        if (cnt != 1) return 0;
    } else {
        if (target.x() == position.x()) {
            int a = target.y(), b = position.y();
            if (a > b) std::swap(a, b);
            for (int i = a + 1; i < b; i++) {
                if (board->existPiece(target.x(), i)) return 0;
            }
        } else {
            int a = target.x(), b = position.x();
            if (a > b) std::swap(a, b);
            for (int i = a + 1; i < b; i++) {
                if (board->existPiece(i, target.y())) return 0;
            }
        }
    }

    return 1;
}

Ma::Ma(Camp camp, QPoint position, QObject *parent) : ChessPiece(camp, position, "Ma", parent) {}

Ma::~Ma() {}

bool Ma::move(ChessBoard *board, QPoint target)
{
    if (target == this->position) return 0;
    ChessPiece *targetPiece = board->getPiece(target);
    if (targetPiece != Q_NULLPTR && targetPiece->getCamp() == this->camp) return 0;

    QPoint d = target - this->position;
    if (!((abs(d.x()) == 1 && abs(d.y()) == 2) || (abs(d.x()) == 2 && abs(d.y()) == 1))) return 0;

    QPoint v;
    if (abs(d.x()) == 2) {
        v.setY(0);
        v.setX(d.x() < 0 ? -1 : 1);
    } else {
        v.setX(0);
        v.setY(d.y() < 0 ? -1 : 1);
    }
    if (board->existPiece(position + v)) return 0;

    return 1;
}
