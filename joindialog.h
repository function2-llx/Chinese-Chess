#ifndef JOINDIALOG_H
#define JOINDIALOG_H

#include <QDialog>
#include <QHostAddress>

class QLineEdit;

namespace Ui {
class JoinDialog;
}

class JoinDialog : public QDialog
{
    Q_OBJECT

public:
    explicit JoinDialog(QWidget *parent = 0);
    ~JoinDialog();

signals:
    void joinHost(QHostAddress, int port);

private slots:

    void on_cancle_pushButton_clicked();

    void on_OK_pushButton_clicked();

    void input(int);

protected:
    bool eventFilter(QObject *, QEvent *);

private:
    Ui::JoinDialog *ui;

    void addText(const QString&);

    void backSpace();

    QLineEdit *focusLineEdit;
};

#endif // JOINDIALOG_H
