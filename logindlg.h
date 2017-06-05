#ifndef LOGINDLG_H
#define LOGINDLG_H
#include <QEvent>
#include <QDialog>
#include <QtGui>
namespace Ui {
class logindlg;
}

class logindlg : public QDialog
{
    Q_OBJECT

public:
    explicit logindlg(QWidget *parent = 0);
    ~logindlg();

private slots:
    void on_loginBtn_clicked();

    void on_exitBtn_clicked();

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::logindlg *ui;
};

#endif // LOGINDLG_H
