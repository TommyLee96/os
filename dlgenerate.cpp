#include "dlgenerate.h"
#include "ui_dlgenerate.h"
#include <QWidget>
DlGenerate::DlGenerate(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DlGenerate)
{
    ui->setupUi(this);
    this->setWindowTitle(tr("生成进程"));

    QValidator *Vint=new QIntValidator(1,1000,this);
    ui->lineEditArrival->setValidator(Vint);
    ui->lineEditCount->setValidator(Vint);
    ui->lineEditPriority->setValidator(Vint);
    ui->lineEditServeTime->setValidator(Vint);
    this->setWindowFlags(windowFlags() &~ Qt::WindowCloseButtonHint&~Qt::WindowContextHelpButtonHint);
}

DlGenerate::~DlGenerate()
{
    delete ui;
}

void DlGenerate::on_pushButtonCancel_clicked()
{
    ui->lineEditArrival->clear();
    ui->lineEditCount->clear();
    ui->lineEditName->clear();
    ui->lineEditPriority->clear();
    ui->lineEditServeTime->clear();
    this->close();
}

void DlGenerate::on_pushButtonOK_clicked()
{
    this->close();
}
