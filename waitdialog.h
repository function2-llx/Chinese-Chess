#ifndef WAITDIALOG_H
#define WAITDIALOG_H

#include <QDialog>

class QLabel;
class QHostAddress;

namespace Ui {
class WaitDialog;
}

class WaitDialog : public QDialog
{
    Q_OBJECT

public:
    explicit WaitDialog(QWidget *parent = 0);
    ~WaitDialog();

    void setHost(const QHostAddress &host, int port = 14514);

private slots:
    void on_pushButton_clicked();

private:
    Ui::WaitDialog *ui;

    QLabel *hostLabel;
};

#endif // WAITDIALOG_H
