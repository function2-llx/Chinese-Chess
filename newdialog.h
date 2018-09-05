#ifndef NEWDIALOG_H
#define NEWDIALOG_H

#include <QDialog>
#include <QMessageBox>
#include <QHostAddress>

namespace Ui {
class NewDialog;
}

class NewDialog : public QDialog
{
    Q_OBJECT

public:
    explicit NewDialog(QWidget *parent = 0, QHostAddress host = QHostAddress::LocalHost);
    ~NewDialog();

private slots:

    void on_OK_pushButton_clicked();

    void on_cancle_pushButton_clicked();

private:
    Ui::NewDialog *ui;



};

#endif // NEWDIALOG_H
