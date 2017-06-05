#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include<QMessageBox>
#include<QTableWidget>
#include<QTableWidgetItem>
#include "dlgenerate.h"
#include <QVector>
#include <QTime>
#include <QTimer>
#include <QMovie>
#include <QProgressBar>
struct DATA {
    QString processName;//进程名
    int serveTime;//服务时间
    int priority;//优先级
    int arriveTime;//到达时间
    int finishTime;//完成时间
    int turnTime;//周转时间
    float turnWeightTime;//带权周转时间
};
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    //friend class DlGenerate;
    void init();//初始化控件
    void CrtProcess(int n=1);//创建指令队列
    void Execute();//开始执行
    void SinglStepExecte();//单步执行
    int MLFB();//多级反馈 Multi-level feedback
    //int FAFS();//先到先服务 First arrive first serve
    int HPLS();//优先级高者服务High priority level service
    void SearchAndAlter(QVector<DATA> &_vector, const DATA _queue);
    void ShowQueue();//显示队列中的进程
  //  void ShowCPU(const int _cpu);//显示CPU状态
    void Show();//显示
    void Clean();//清屏
    void Reset();//重置
    void Pause();//暂停
    void ShowResult();
    void Exit();//退出
    void ShowResult(int index);
    void RefreshInfo(QTableWidget *tablewidgetBfr,int index=-1);
    void MoveItem(QTableWidget *source, QTableWidget *target, int durtime);
    void InitTable();
    void Initheaders();
    void InitProgressBar();
    void ShowProgress(QProgressBar *protmp, int tslice);
public slots:

    void slicecomplete();
    void recvtimer();
private slots:
    void on_pushButtonexit_clicked();

    void on_pushButtonGenerate_clicked();

    void on_tableWidget1st_currentItemChanged(QTableWidgetItem *current, QTableWidgetItem *previous);

    void onTimerOut();

    void on_pushButtonStart_clicked();

    void on_tableWidgetProcess_currentItemChanged(QTableWidgetItem *current, QTableWidgetItem *previous);

    void on_tableWidget2nd_currentItemChanged(QTableWidgetItem *current, QTableWidgetItem *previous);

    void on_tableWidget3rd_currentItemChanged(QTableWidgetItem *current, QTableWidgetItem *previous);




    void on_pushButtonReset_clicked();



private:
    Ui::MainWindow *ui;
    DlGenerate *dlgn;   //添加私有成员，为一个dailog（窗口）的指针
    //DialogBlock *dialog2;
    QVector<DATA>myQueue[3];//多级反馈队列
    //QVector<DATA>processVector[3];//进程
    QVector<DATA>processVector;//进程
    int a[3];//时间片
    int CPU;//CPU状态
    float speed;//时间流速
    int nCount;//进程总数
    int executeTime;//已执行时间
    int nextProPoint;//下一个进程位置
    QMovie *movieCpu;
    int CurSpeed;
    QTableWidget *twResult;
    QTimer *tmr_1,*tmr_2;
    QProgressBar *curProgress;

    QString strAlgorithm;
    int noReachPoint,noReachPoint2;

};

#endif // MAINWINDOW_H
