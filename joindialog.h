#ifndef JOINDIALOG_H
#define JOINDIALOG_H

#include <QDialog>
#include <QHostAddress>

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
    void joinHost(QHostAddress);

private slots:

    void on_cancle_pushButton_clicked();

    void on_OK_pushButton_clicked();

    void input(int);


private:
    Ui::JoinDialog *ui;

    void addText(const QString&);

    void backSpace();
};

#endif // JOINDIALOG_H
