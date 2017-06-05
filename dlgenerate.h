#ifndef DLGENERATE_H
#define DLGENERATE_H

#include <QDialog>

namespace Ui {
class DlGenerate;
}

class DlGenerate : public QDialog
{
    Q_OBJECT

public:
    explicit DlGenerate(QWidget *parent = 0);
    ~DlGenerate();
    friend class MainWindow;
private slots:
    void on_pushButtonCancel_clicked();

    void on_pushButtonOK_clicked();

private:
    Ui::DlGenerate *ui;
};

#endif // DLGENERATE_H
