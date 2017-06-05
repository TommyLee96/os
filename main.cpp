#include "mainwindow.h"
#include "logindlg.h"
#include <QApplication>
#include <QPixmap>
#include <QLabel>
#include <QMovie>
#include <QtGui>
#include <QDesktopWidget>
#include <QSplashScreen>
#include <iostream>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    logindlg login;
       if(login.exec()==QDialog::Accepted)
       {
           qApp->processEvents();
           w.show();
           return a.exec();
       }
       else return 0;


}
