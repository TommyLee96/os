#include "logindlg.h"
#include "ui_logindlg.h"
#include <QEvent>
#include <QtGui>
#include<QMessageBox>
#include <QMovie>

logindlg::logindlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::logindlg)
{
    ui->setupUi(this);
    this->setWindowIcon(QIcon(":/favicon.ico"));
    ui->pwdLineEdit->setEchoMode( QLineEdit::Password );
    this->setWindowFlags(Qt::FramelessWindowHint);
    ui->label->setScaledContents(true);
    QMovie *movie1 = new QMovie(":/5.GIF");
    ui->label->setMovie(movie1);
    movie1->start();

}

logindlg::~logindlg()
{
    delete ui;
}

void logindlg::on_loginBtn_clicked()
{
    // 判断用户名和密码是否正确，
    // 如果错误则弹出警告对话框
    if(ui->usrLineEdit->text() == tr("qt") &&
           ui->pwdLineEdit->text() == tr("123456"))
    {
       accept();
    }
    else {

        QMessageBox::warning(this,tr("warn"),tr("用户名或者密码输错啦 !!!!!!!!!"),QMessageBox::Yes);
    }
}

void logindlg::on_exitBtn_clicked()
{

}

/*
void logindlg::mousePressEvent(QMouseEvent *event)
{
     this->windowPos = this->pos();                // 获得部件当前位置
     this->mousePos = event->globalPos();     // 获得鼠标位置
     this->dPos = mousePos - windowPos;       // 移动后部件所在的位置
}

void logindlg::mouseMoveEvent(QMouseEvent *event)
{
     this->move(event->globalPos() - this->dPos);
}
*/

void logindlg::on_pushButton_clicked()
{
    exit;
}

void logindlg::on_pushButton_2_clicked()
{
    QMessageBox message(QMessageBox::NoIcon, "提示", "user： qt\n\rpassword：123456");
    message.setIconPixmap(QPixmap(":/tips.png"));
    message.exec();
}
