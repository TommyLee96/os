#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ui_dlgenerate.h"
#include <QValidator>
#include <QDebug>
#include <QHeaderView>
#include <QMessageBox>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QAbstractItemView>
#include <QDialog>
#include <QPixmap>
#include <QMovie>
#include <QStyle>

#include <QPropertyAnimation>

#include <iostream>
using namespace std;
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowIcon(QIcon(":/favicon.ico"));
    CPU=-1;
    this->setWindowTitle(tr("多级反馈调度算法演示"));
    a[0]=2;a[1]=4;a[2]=6;
    executeTime=0;
    noReachPoint=0;
    noReachPoint2=0;
    ui->lineEdit1st->setText(tr("2"));  //初始时间片sh设置
    ui->lineEdit2nd->setText(tr("4"));  //
    ui->lineEdit3rd->setText(tr("6"));  //
    nextProPoint=0;

    QValidator *Vint=new QIntValidator(1,1000,this);
    ui->lineEdit1st->setValidator(Vint);
    ui->lineEdit2nd->setValidator(Vint);
    ui->lineEdit3rd->setValidator(Vint);
    ui->pushButtonStart->setEnabled(false);
    dlgn=NULL;
    twResult=NULL;
    ui->lcdNumber->setDigitCount(7);
    nCount=0;
    InitTable();
    Initheaders();
    InitProgressBar();
    tmr_2=new QTimer(this);
    tmr_1=new QTimer(this);
    tmr_1->setSingleShot(true);
    tmr_2->setInterval(10);
    tmr_2->setSingleShot(false);
    connect(tmr_1,SIGNAL(timeout()),this,SLOT(slicecomplete()));// border-style:solid;


    connect(tmr_2, SIGNAL(timeout()), this, SLOT(recvtimer()));
   // ui->tableWidget1st->setStyleSheet("tableWidget1st::item{setForeground(QBrush(QColor(255, 0, 0))); border-style:solid;background: transparent; }");
    ui->horizontalSlider->setRange(1,15);
    ui->horizontalSlider->setSingleStep(1);
    ui->horizontalSlider->setTickInterval(1);


}

MainWindow::~MainWindow()
{
    delete ui;
    if(twResult!=NULL)
    {
        delete twResult;
    }
}

void MainWindow::InitTable()
{

    ui->tableWidget1st->setRowCount(0);
    ui->tableWidget2nd->setRowCount(0);
    ui->tableWidget3rd->setRowCount(0);
    ui->tableWidgetProcess->setRowCount(0);
    //ui->tableWidgetFIFO->setRowCount(0);
   // ui->tableWidgetPriority->setRowCount(0);

    ui->tableWidget1st->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);//设置单元格大小自适应内容
    ui->tableWidget2nd->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget3rd->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidgetProcess->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
   // ui->tableWidgetFIFO->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
   // ui->tableWidgetPriority->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    ui->tableWidget1st->setEditTriggers(QAbstractItemView::NoEditTriggers);//设置表格只读
    ui->tableWidget2nd->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget3rd->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidgetProcess->setEditTriggers(QAbstractItemView::NoEditTriggers);
   // ui->tableWidgetFIFO->setEditTriggers(QAbstractItemView::NoEditTriggers);
  // ui->tableWidgetPriority->setEditTriggers(QAbstractItemView::NoEditTriggers);

    ui->tableWidget1st->setSelectionBehavior(QAbstractItemView::SelectRows);//设置表格按行选中
    ui->tableWidget2nd->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget3rd->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidgetProcess->setSelectionBehavior(QAbstractItemView::SelectRows);
   // ui->tableWidgetFIFO->setSelectionBehavior(QAbstractItemView::SelectRows);
   // ui->tableWidgetPriority->setSelectionBehavior(QAbstractItemView::SelectRows);

    ui->tableWidget1st->setSelectionMode(QAbstractItemView::SingleSelection);//设置表格单选
    ui->tableWidget2nd->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableWidget3rd->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableWidgetProcess->setSelectionMode(QAbstractItemView::SingleSelection);
   // ui->tableWidgetFIFO->setSelectionMode(QAbstractItemView::SingleSelection);
   // ui->tableWidgetPriority->setSelectionMode(QAbstractItemView::SingleSelection);

    ui->tableWidgetInfo->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidgetInfo->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidgetInfo->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidgetInfo->setRowCount(5);
    ui->tableWidgetInfo->setColumnCount(1);
    ui->tableWidgetInfo->horizontalHeader()->setVisible(false);//隐藏信息表格的水平表头

    ui->tableWidgetInfo->verticalHeader()->setStyleSheet("QHeaderView::section { background-color:  rgb(175, 238 ,238);}");

    ui->tableWidgetProcess->verticalHeader()->setStyleSheet("QHeaderView::section { background-color:  rgb(175, 238 ,238);}");
    ui->tableWidgetProcess->horizontalHeader()->setStyleSheet("QHeaderView::section { background:transparent;}");

   // ui->tableWidgetFIFO->verticalHeader()->setStyleSheet("QHeaderView::section { background-color: rgb(0 ,238 ,118);}");
   // ui->tableWidgetFIFO->horizontalHeader()->setStyleSheet("QHeaderView::section { background:transparent;}");

   // ui->tableWidget1st->horizontalHeader()->setStyleSheet("QHeaderView::section { background-color: rgb(0 ,238 ,118);}");
    ui->tableWidget1st->horizontalHeader()->setStyleSheet("QHeaderView::section { background:transparent;}");

  //  ui->tableWidget2nd->verticalHeader()->setStyleSheet("QHeaderView::section { background-color: rgb(0 ,238 ,118);}");
    ui->tableWidget2nd->horizontalHeader()->setStyleSheet("QHeaderView::section { background:transparent;}");

   // ui->tableWidget3rd->verticalHeader()->setStyleSheet("QHeaderView::section { background-color: rgb(0 ,238 ,118);}");
    ui->tableWidget3rd->horizontalHeader()->setStyleSheet("QHeaderView::section { background:transparent;}");

    //ui->tableWidgetPriority->verticalHeader()->setStyleSheet("QHeaderView::section { background-color: rgb(255, 255, 255);}");
    //ui->tableWidgetPriority->horizontalHeader()->setStyleSheet("QHeaderView::section { background:transparent;}");
}

void MainWindow::Initheaders()
{
    ui->tableWidget1st->setColumnCount(1);//设置表格列数
    ui->tableWidget2nd->setColumnCount(1);
    ui->tableWidget3rd->setColumnCount(1);
    //ui->tableWidgetFIFO->setColumnCount(1);
   // ui->tableWidgetPriority->setColumnCount(1);
    //han
    ui->tableWidgetProcess->setColumnCount(1);
    QStringList header1;
    header1<<tr("进程名");
    ui->tableWidget1st->setHorizontalHeaderLabels(header1);//设置表格表头
    ui->tableWidget2nd->setHorizontalHeaderLabels(header1);
    ui->tableWidget3rd->setHorizontalHeaderLabels(header1);
    //ui->tableWidgetFIFO->setHorizontalHeaderLabels(header1);
  //  ui->tableWidgetPriority->setHorizontalHeaderLabels(header1);


    QStringList header2;
    header2<<tr("进程名")<<tr("开始时间")<<tr("总服务时间")<<tr("剩余服务时间")<<tr("优先级");//优先级只在优先级调度时才显示
    ui->tableWidgetInfo->setVerticalHeaderLabels(header2);

    //han
    QStringList headerProcess;
    headerProcess<<tr("初始");

    ui->tableWidget1st->verticalHeader()->setVisible(false);
     ui->tableWidget2nd->verticalHeader()->setVisible(false);
      ui->tableWidget3rd->verticalHeader()->setVisible(false);  //把表的行头设置不可见

  //  self.MyTable.verticalHeader().setVisible(False)
    ui->tableWidgetProcess->setHorizontalHeaderLabels(headerProcess);
}

void MainWindow::InitProgressBar()
{
    ui->progressBar1st->setValue(0);
    ui->progressBar1st->setVisible(false);
    ui->progressBar2nd->setValue(0);
    ui->progressBar2nd->setVisible(false);
    ui->progressBar3rd->setValue(0);
    ui->progressBar3rd->setVisible(false);
  //  ui->progressBarFIFO->setValue(0);
  //  ui->progressBarFIFO->setVisible(false);
   // ui->progressBarPOI->setValue(0);
   // ui->progressBarPOI->setVisible(false);
}

void MainWindow::SearchAndAlter(QVector<DATA> &_vector, const DATA _queue)
{
    int i = -1;
    /*cout << (_vector[++i].processName.compare(_queue.processName) == 0) << endl;
    cout << (_vector[i].processName.compare("Pro1") == 0) << endl;*/
    int tempFlag = -1;
    QString s1 = _queue.processName;
    while (tempFlag != 0)
    {
        tempFlag = _vector[++i].processName.compare(s1);
    }
    _vector[i].finishTime = _queue.finishTime;
    _vector[i].turnTime = _vector[i].finishTime - _vector[i].arriveTime;
    _vector[i].turnWeightTime = (float)_vector[i].turnTime / (float)_vector[i].serveTime;
    return;

}

int MainWindow::MLFB()
{
    if (processVector.size() == 0)
    {
        return -1;
    }
    if(ui->tableWidget1st->rowCount()==0 && ui->tableWidget2nd->rowCount()==0 && ui->tableWidget3rd->rowCount()==0)
    {
        if (nextProPoint<processVector.size() && executeTime <= processVector[nextProPoint].arriveTime)
        {
           CPU=-1;
            RefreshInfo(ui->tableWidgetProcess,0);
            ShowProgress(NULL,processVector[nextProPoint].arriveTime-executeTime);
            executeTime = processVector[nextProPoint].arriveTime;
        }
    }


    //han2017/2/27-12:36
    while (ui->tableWidgetProcess->rowCount()>0 && nextProPoint != processVector.size() &&
    executeTime >= processVector[nextProPoint].arriveTime
    )
    {//新进入进程
        RefreshInfo(ui->tableWidgetProcess,0);
        myQueue[0].push_back(processVector[nextProPoint]);
        nextProPoint++;
        MoveItem(ui->tableWidgetProcess,ui->tableWidget1st,300);

        return 0;
    }
    bool flag = false;
    for (int i = 0;i < 3 && flag==false;i++)
    {
        while (!myQueue[i].empty() && flag==false)
        {
            CPU = i;
            int nsleep=(myQueue[i].front().serveTime<a[i]?
                            myQueue[i].front().serveTime:a[i]);

            ui->tableWidget1st->setCurrentCell(-1,-1);
            ui->tableWidget2nd->setCurrentCell(-1,-1);
            ui->tableWidget3rd->setCurrentCell(-1,-1);
            if(CPU==0)
            {
                RefreshInfo(ui->tableWidget1st,0);
                ShowProgress(ui->progressBar1st,nsleep);
            }
            else if(CPU==1)
            {
                RefreshInfo(ui->tableWidget2nd,0);
                //ui->tableWidget2nd->setCurrentCell(-1,-1);
                ShowProgress(ui->progressBar2nd,nsleep);
            }
            else if(CPU==2)
            {
                RefreshInfo(ui->tableWidget3rd,0);
                //ui->tableWidget3rd->setCurrentCell(-1,-1);
                ShowProgress(ui->progressBar3rd,nsleep);
            }

            //ShowCPU(CPU);
            if (myQueue[i].front().serveTime > a[i])
            {//未执行完
                myQueue[i].front().serveTime -= a[i];
                executeTime += a[i];

                if (i < 2)
                {//压入下一个队列

                    if(i==0)
                    {
                        MoveItem(ui->tableWidget1st,ui->tableWidget2nd,300);
                    }
                    else
                    {
                        MoveItem(ui->tableWidget2nd,ui->tableWidget3rd,300);
                    }

                    DATA temp = myQueue[i].front();
                    myQueue[i + 1].push_back(temp);
                    QVector<DATA>::Iterator it;
                    it = myQueue[i].begin();
                    myQueue[i].erase(it);
                }
                if (i == 2)
                {//在本队列中轮转

                    MoveItem(ui->tableWidget3rd,ui->tableWidget3rd,300);
                    DATA temp = myQueue[i].front();
                    myQueue[i].push_back(temp);
                    QVector<DATA>::iterator it;
                    it = myQueue[i].begin();
                    myQueue[i].erase(it);
                }
            }
            else if (myQueue[i].front().serveTime <= a[i])
            {//执行完毕
                executeTime += myQueue[i].front().serveTime;//当前时间
                myQueue[i].front().serveTime = 0;//服务时间
                myQueue[i].front().finishTime = executeTime;//完成时间
                //出队
                DATA temp = myQueue[i].front();
                QVector<DATA>::Iterator it;
                it = myQueue[i].begin();
                myQueue[i].erase(it);
                SearchAndAlter(processVector,temp);//更新数据


            }
            //han2017/3/27-12:47
            //qDebug()<<executeTime<<endl;
            flag = true;

            CPU = -1;
            break;
        }
    }
    if (myQueue[0].empty() && myQueue[1].empty() && myQueue[2].empty())
    {
        return 1;
    }
    return 0;
}

void MainWindow::on_pushButtonStart_clicked()
{//开始执行

    if(ui->tabWidget->currentIndex()==0)
    {
        ui->tabWidget->setTabEnabled(1,false);
        ui->tabWidget->setTabEnabled(2,false);
        strAlgorithm=tr("多级反馈队列调度算法");
        int a1=ui->lineEdit1st->text().toInt();
        int a2=ui->lineEdit2nd->text().toInt();
        int a3=ui->lineEdit3rd->text().toInt();

        if(a1<a2&&a2<a3)
        {
            a[0]=a1;
            a[1]=a2;
            a[2]=a3;
            ui->lineEdit1st->setReadOnly(true);
            ui->lineEdit2nd->setReadOnly(true);
            ui->lineEdit3rd->setReadOnly(true);
        }
        else
        {
            QMessageBox::information(this,tr("无法执行"),tr("时间片要求1级队列<2级队列<3级队列"));
            return;
        }


        while(true)
        {

            if(ui->tableWidgetProcess->rowCount()>0||
            ui->tableWidget1st->rowCount()>0||
            ui->tableWidget2nd->rowCount()>0||
            ui->tableWidget3rd->rowCount()>0)
            {
                ui->pushButtonStart->setEnabled(false);
                ui->pushButtonReset->setEnabled(false);
                MLFB();
                ShowQueue();
                QTime t;
                t.start();
                while(t.elapsed()<1)
                {
                     QCoreApplication::processEvents();
                }
            }
              else
                  break;

        }
    }


    RefreshInfo(NULL,2);//显示空
    //ShowCPU(CPU);
    ui->pushButtonStart->setEnabled(true);
    ui->pushButtonReset->setEnabled(true);
    //qDebug()<<"Calculation complete.";
    ShowResult();
    //ui->tabWidget->setEnabled(true);
}



bool CompareByArrivetime(const DATA &a,const DATA &b)//到达时间排序
{
    return a.arriveTime<b.arriveTime;
}

bool CompareByPriority(const DATA &a,const DATA &b)//优先级排序
 {
    if(a.arriveTime==b.arriveTime)
    {
      return a.priority <b.priority;
    }
    else
    {
        return a.arriveTime<b.arriveTime;
    }
 }



void MainWindow::on_pushButtonexit_clicked()
{
    exit(0);
}
void MainWindow::CrtProcess(int n)
{
    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));
    int tempN=nCount;
    for(int i=tempN;i<n+tempN;i++)
    {
        QString aName=tr("process%1").arg(i+1);
        int aServetime=qrand() % 40 + 1;
        int aPriority=qrand() % 7 + 1;

        int aArrival= executeTime;

        DATA ad={aName,aServetime,aPriority,aArrival,-1,-1,-1};

        if(processVector.size()>0)
        {
            QVector<DATA>::const_iterator it;
            for(it=processVector.begin();it!=processVector.end();it++)
            {
                if(it->processName.compare(aName)==0)
                    break;
            }
            if(it!=processVector.end())
            {
                n++;

                continue;
            }
        }
        nCount++;
        processVector.append(ad);


    }
}

void MainWindow::on_pushButtonGenerate_clicked()
{
    if(dlgn==NULL)
    {
        dlgn=new DlGenerate(this);

    }
    dlgn->exec();
    /*else
    {
        dlgn->show();
    }*/
    if(dlgn->ui->tabWidget->currentIndex()==0)//选择随机生成
    {
        if(!dlgn->ui->lineEditCount->currentText().isEmpty())
        {
            int n=dlgn->ui->lineEditCount->currentText().toInt();//获取要生成的进程个数
            CrtProcess(n);

            ui->pushButtonStart->setEnabled(true);
        }

    }
    else
    {
        if((!dlgn->ui->lineEditArrival->text().isEmpty())&&(!dlgn->ui->lineEditName->text().isEmpty())&&(!dlgn->ui->lineEditPriority->text().isEmpty())&&(!dlgn->ui->lineEditServeTime->text().isEmpty()))
        {
            int mArrival=dlgn->ui->lineEditArrival->text().toInt();
            QString mName=dlgn->ui->lineEditName->text().toUtf8();
            int mServeTime=dlgn->ui->lineEditServeTime->text().toInt();
            int mPriority=dlgn->ui->lineEditPriority->text().toInt();
            DATA md={mName,mServeTime,mPriority,mArrival,-1,-1,-1};
            //han判断是否有相同进程
            if(processVector.size()==0)
            {
                processVector.append(md);
                nCount++;
                //sort(processVector.begin()+nextProPoint,processVector.end(),CompareByArrivetime);
            }
            //han2017.4.3.18:55

            else if(processVector.size()>0)
            {
                QVector<DATA>::const_iterator it;
                for(it=processVector.begin();it!=processVector.end();it++)
                {
                    if(it->processName.compare(mName)==0)
                        break;
                }
                if(it!=processVector.end())
                {
                    QMessageBox::information(this,tr("提示"),tr("%1进程相同").arg(mName));
                }
                else
                {
                    processVector.append(md);
                    nCount++;
                }
            }
            ui->pushButtonStart->setEnabled(true);
        }

    }
    init();
    delete dlgn;
    dlgn=NULL;
}
void MainWindow::init()
{

    QVector<DATA>::const_iterator it;
    it=processVector.begin();

    sort(processVector.begin()+nextProPoint,processVector.end(),CompareByPriority);
    for(int i=nextProPoint;i<nCount;i++)
    {
        int point=i-nextProPoint;
        ui->tableWidgetProcess->setRowCount(point+1);
        ui->tableWidgetProcess->setItem(point,0,new QTableWidgetItem((it+i)->processName));
        ui->tableWidgetProcess->item(point,0)->setTextAlignment(Qt::AlignCenter);//设置单元格内容居中显示
    }
    if(ui->tableWidgetProcess->rowCount()>0)
        RefreshInfo(ui->tableWidgetProcess,0);
}

void MainWindow::ShowQueue()
{

    for(int i=0;i<3;i++)
    {
        QVector<DATA>::const_iterator it;
        it=myQueue[i].begin();

        for(int j=0;j<myQueue[i].size();j++)
        {

            if(i==0)
            {
                ui->tableWidget1st->setRowCount(myQueue[i].size());
                ui->tableWidget1st->setItem(j,0,new QTableWidgetItem((it+j)->processName));
                ui->tableWidget1st->item(j,0)->setTextAlignment(Qt::AlignCenter);
               ui->tableWidget1st->item(j,0)->setBackgroundColor(QColor(255 ,127 ,0));
             ui->tableWidget1st->item(j,0)->setFont(QFont("Microsoft YaHei", 12,75));
             ui->tableWidget1st->item(j,0)->setTextColor(QColor(0,0,0));
            }

            if(i==1)
            {
                ui->tableWidget2nd->setRowCount(j+1);
                ui->tableWidget2nd->setItem(j,0,new QTableWidgetItem((it+j)->processName));
                ui->tableWidget2nd->item(j,0)->setTextAlignment(Qt::AlignCenter);
                ui->tableWidget2nd->item(j,0)->setBackgroundColor(QColor(255 ,127, 0));
                 ui->tableWidget2nd->item(j,0)->setFont(QFont("Microsoft YaHei", 12,75));
                 ui->tableWidget2nd->item(j,0)->setTextColor(QColor(0 ,0,0));
            }
            if(i==2)
            {
                ui->tableWidget3rd->setRowCount(j+1);
                ui->tableWidget3rd->setItem(j,0,new QTableWidgetItem((it+j)->processName));
                ui->tableWidget3rd->item(j,0)->setTextAlignment(Qt::AlignCenter);
                ui->tableWidget3rd->item(j,0)->setBackgroundColor(QColor(255, 127 ,0));
                ui->tableWidget3rd->item(j,0)->setFont(QFont("Microsoft YaHei", 12,75));
                ui->tableWidget3rd->item(j,0)->setTextColor(QColor(0 ,0,0));
            }

        }

        if(myQueue[i].size()==0)
        {
            if(i==0)
                ui->tableWidget1st->setRowCount(0);
            else if(i==1)
                ui->tableWidget2nd->setRowCount(0);
            else
                ui->tableWidget3rd->setRowCount(0);
        }

    }

}

void MainWindow::RefreshInfo(QTableWidget *tablewidgetBfr,int index)
{
    QWidget *curFocus=QApplication::focusWidget();
    if(index==0|| curFocus==ui->tableWidgetProcess||curFocus==ui->tableWidget1st||curFocus==ui->tableWidget2nd||curFocus==ui->tableWidget3rd)
    {
        if(index==-1)
            tablewidgetBfr=qobject_cast<QTableWidget *>(curFocus);
        if(index==0 || tablewidgetBfr->currentRow()>=0)
        {
            QVector<DATA>::const_iterator it=processVector.begin();

            int nCur=tablewidgetBfr->currentRow();

            if(index==-1)
                nCur=tablewidgetBfr->currentRow();
            else
                nCur=0;


            int i = -1;

            int tempFlag = -1;

            QString s1 = tablewidgetBfr->item(nCur,0)->text();
            while (tempFlag != 0)
            {
                tempFlag = processVector[++i].processName.compare(s1);
            }

            //剩余服务时间
            int remainTime=processVector[i].serveTime;
            if(tablewidgetBfr==ui->tableWidget1st)
            {
                remainTime=myQueue[0][nCur].serveTime;
            }
            else if(tablewidgetBfr==ui->tableWidget2nd)
            {
                remainTime=myQueue[1][nCur].serveTime;
            }
            else if(tablewidgetBfr==ui->tableWidget3rd)
            {
                remainTime=myQueue[2][nCur].serveTime;
            }
            else
            {
                remainTime=processVector[i].serveTime;
            }



            ui->tableWidgetInfo->setItem(0,0,new QTableWidgetItem(processVector[i].processName));
            ui->tableWidgetInfo->item(0,0)->setTextAlignment(Qt::AlignCenter);
            ui->tableWidgetInfo->setItem(1,0,new QTableWidgetItem(tr("%1").arg(processVector[i].arriveTime)));
            ui->tableWidgetInfo->item(1,0)->setTextAlignment(Qt::AlignCenter);
            ui->tableWidgetInfo->setItem(2,0,new QTableWidgetItem(tr("%1").arg(processVector[i].serveTime)));
            ui->tableWidgetInfo->item(2,0)->setTextAlignment(Qt::AlignCenter);
            ui->tableWidgetInfo->setItem(3,0,new QTableWidgetItem(tr("%1").arg(remainTime)));
            ui->tableWidgetInfo->item(3,0)->setTextAlignment(Qt::AlignCenter);
            ui->tableWidgetInfo->setItem(4,0,new QTableWidgetItem(tr("%1").arg(processVector[i].priority)));
            ui->tableWidgetInfo->item(4,0)->setTextAlignment(Qt::AlignCenter);
        }

    }
    if(index==2)
    {
        ui->tableWidgetInfo->setItem(0,0,new QTableWidgetItem(tr(" ")));
        ui->tableWidgetInfo->item(0,0)->setTextAlignment(Qt::AlignCenter);
        ui->tableWidgetInfo->setItem(1,0,new QTableWidgetItem(tr(" ")));
        ui->tableWidgetInfo->item(1,0)->setTextAlignment(Qt::AlignCenter);
        ui->tableWidgetInfo->setItem(2,0,new QTableWidgetItem(tr(" ")));
        ui->tableWidgetInfo->item(2,0)->setTextAlignment(Qt::AlignCenter);
        ui->tableWidgetInfo->setItem(3,0,new QTableWidgetItem(tr(" ")));
        ui->tableWidgetInfo->item(3,0)->setTextAlignment(Qt::AlignCenter);
        ui->tableWidgetInfo->setItem(4,0,new QTableWidgetItem(tr(" ")));
        ui->tableWidgetInfo->item(4,0)->setTextAlignment(Qt::AlignCenter);
    }


}

void MainWindow::MoveItem(QTableWidget *source,QTableWidget *target,int durtime)
{

    if(source==target && target->rowCount()==1)
        return;

    QPoint globalPoint1(source->mapTo(this,source->pos()));
    QPoint globalPoint2(target->mapTo(this,target->pos()));

    int xstart=globalPoint1.x()+source->verticalHeader()->width();
    int ystart=globalPoint1.y();
    int xend=globalPoint2.x();
    int yend=globalPoint2.y();
    for(int i=0;i<target->rowCount()&& i<14;i++)
    {

            yend+=source->rowHeight(0);

    }
    if(source==target)
        yend-=source->rowHeight(0);


    ystart=89;



    source->removeRow(0);

    QTime t;
    t.start();
    while(t.elapsed()<durtime)
    {
         QCoreApplication::processEvents();
    }

}


void MainWindow::onTimerOut()
{
    /*
    //QTime t=QTime::currentTime();
    int hh=0,mm=0,ss=0,curtime=(tm->elapsed()+500)/1000;
    QString strRun;
    ss=curtime%60;
    curtime=(curtime-ss)/60;
    mm=curtime%60;
    curtime=(curtime-mm)/60;
    hh=curtime;
    strRun.append(tr("%1:").arg(hh));
    if(mm<10)
    {
        strRun.append(tr("0"));
    }
    strRun.append(tr("%1:").arg(mm));
    if(ss<10)
    {
        strRun.append(tr("0"));

    }
    strRun.append(tr("%1").arg(ss));
    //ui->lcdNumber->display(tr("%1:%2:%3").arg(hh).arg(mm).arg(ss));
    ui->lcdNumber->display(strRun);
    */
}

void MainWindow::on_tableWidget1st_currentItemChanged(QTableWidgetItem *current, QTableWidgetItem *previous)
{
    RefreshInfo(ui->tableWidget1st);
}



void MainWindow::on_tableWidgetProcess_currentItemChanged(QTableWidgetItem *current, QTableWidgetItem *previous)
{
    RefreshInfo(ui->tableWidgetProcess);
}

void MainWindow::on_tableWidget2nd_currentItemChanged(QTableWidgetItem *current, QTableWidgetItem *previous)
{
    RefreshInfo(ui->tableWidget2nd);
}

void MainWindow::on_tableWidget3rd_currentItemChanged(QTableWidgetItem *current, QTableWidgetItem *previous)
{
    RefreshInfo(ui->tableWidget3rd);
}

void MainWindow::slicecomplete()
{
    curProgress->setValue(0);
    tmr_2->stop();
    curProgress->setVisible(false);
    //qDebug()<<tr("slice comlpete");
}

void MainWindow::recvtimer()
{

    int msnow=tmr_1->interval()-tmr_1->remainingTime();
    curProgress->setValue(msnow);
    int hh=0,mm=0,ss=0,curtime=executeTime+(msnow+500/CurSpeed)*CurSpeed/1000;
    QString strRun;
    ss=curtime%60;
    curtime=(curtime-ss)/60;
    mm=curtime%60;
    curtime=(curtime-mm)/60;
    hh=curtime;
    strRun.append(tr("%1:").arg(hh));
    if(mm<10)
    {
        strRun.append(tr("0"));
    }
    strRun.append(tr("%1:").arg(mm));
    if(ss<10)
    {
        strRun.append(tr("0"));

    }
    strRun.append(tr("%1").arg(ss));

    ui->lcdNumber->display(strRun);
}
void MainWindow::ShowProgress(QProgressBar *protmp, int tslice)
{
    //ShowCPU(CPU);
    int nSpeed=3*ui->horizontalSlider->value();        // 速度改变
    tmr_1->setInterval(tslice*1000/nSpeed);
    curProgress=protmp;

    if(curProgress!=NULL)
    {
        curProgress->setRange(0,tslice*1000/nSpeed);
        CurSpeed=nSpeed;
        curProgress->setVisible(true);
        //protmp->setMaximum(tslice*1000);
        tmr_1->start();
        tmr_2->start();
        protmp->setFocus();
        while(tmr_2->isActive())
        {
            QCoreApplication::processEvents();
        }
    }
    else if(curProgress==NULL)
    {
        if(ui->tabWidget->currentIndex()==0)
            curProgress=ui->progressBar1st;
       // else if(ui->tabWidget->currentIndex()==1)
        //    curProgress=ui->progressBarFIFO;
       // else if(ui->tabWidget->currentIndex()==2)
          //  curProgress=ui->progressBarPOI;
        else return;

        curProgress->setRange(0,tslice*1000/nSpeed);
        CurSpeed=nSpeed;
        curProgress->setVisible(false);
        //protmp->setMaximum(tslice*1000);
        tmr_1->start();
        tmr_2->start();
        while(tmr_2->isActive())
        {
            QCoreApplication::processEvents();
        }
    }

    //t->start();
}

void MainWindow::Reset()
{
    ui->tabWidget->setTabEnabled(0,true);
    ui->tabWidget->setTabEnabled(1,true);
    ui->tabWidget->setTabEnabled(2,true);
    ui->tableWidget1st->clear();
    ui->tableWidget2nd->clear();
    ui->tableWidget3rd->clear();
    //ui->tableWidgetFIFO->clear();
    ui->tableWidgetInfo->clear();
    ui->horizontalSlider->setValue(1);
  //  ui->tableWidgetPriority->clear();
    ui->tableWidgetProcess->clear();
    ui->lcdNumber->display(0);
    ui->pushButtonStart->setEnabled(false);
    InitTable();
    Initheaders();
    InitProgressBar();
    nCount=0;
    executeTime=0;
    nextProPoint=0;
    noReachPoint=0;
    noReachPoint2=0;
    ui->lineEdit1st->setText(tr("2"));
    ui->lineEdit2nd->setText(tr("4"));
    ui->lineEdit3rd->setText(tr("6"));
    processVector.erase(processVector.begin(),processVector.end());
    for(int i=0;i<3;i++)
    {
        myQueue[i].erase(myQueue[i].begin(),myQueue[i].end());
    }
}

void MainWindow::on_pushButtonReset_clicked()
{
    Reset();
}

void MainWindow::ShowResult()
{
    if(twResult==NULL)
        {
            twResult= new QTableWidget();
        }
        else
        {
            twResult->clear();
        }
        int rows=processVector.size();
        twResult->setRowCount(rows);
        twResult->setColumnCount(6);
        twResult->setWindowTitle(tr("结果概览"));
        twResult->setStyleSheet("#QTableWidget{border-image:url(:/resultbg.jpg)}");
        twResult->horizontalHeader()->setStyleSheet("QHeaderView::section { background:transparent}");
        twResult->verticalHeader()->setStyleSheet("QHeaderView::section { background:transparent}");
        QStringList headerResult;
        double aveRun=0,aveWRun=0;
        headerResult<<tr("进程名")<<tr("服务时间")<<tr("开始时间")<<tr("完成时间")<<tr("周转时间")<<tr("带权周转时间");
        twResult->setHorizontalHeaderLabels(headerResult);
        twResult->resizeRowsToContents();
        twResult->setEditTriggers(QAbstractItemView::NoEditTriggers);

        for(int i=0;i<rows;i++)
        {
            twResult->setItem(i,0,new QTableWidgetItem(processVector[i].processName));
           // twResult->setItem(i,1,new QTableWidgetItem(tr("%1").arg(processVector[i].priority)));
            twResult->setItem(i,1,new QTableWidgetItem(tr("%1").arg(processVector[i].serveTime)));
            twResult->setItem(i,2,new QTableWidgetItem(tr("%1").arg(processVector[i].arriveTime)));
            twResult->setItem(i,3,new QTableWidgetItem(tr("%1").arg(processVector[i].finishTime)));
            twResult->setItem(i,4,new QTableWidgetItem(tr("%1").arg(processVector[i].turnTime)));
            twResult->setItem(i,5,new QTableWidgetItem(QString::number(processVector[i].turnWeightTime,'f',2)));
            aveRun+=processVector[i].turnTime;
            aveWRun+=processVector[i].turnWeightTime;
        }
        twResult->resize(twResult->verticalHeader()->width()+twResult->horizontalHeader()->width(),twResult->horizontalHeader()->height()+twResult->verticalHeader()->height());
        aveRun=aveRun*1.0/rows;
        aveWRun=aveWRun*1.0/rows;

       // QMessageBox::information(this,tr("结果统计完毕"),tr("使用算法:%1\n平均周转时间:%2\n带权平均周转时间:%3").arg(strAlgorithm).arg(QString::number(aveRun,'f',2)).arg(QString::number(aveWRun,'f',2)));
        twResult->show();



}
