# QT笔记
* QT开启多线程，需要创建继承QThread类，重写run函数，通过执行start开启线程;
* *多线程访问时需考虑数据读取先后顺序，可通过在数据块前后添加QMutex；
QMutex类提供了线程之间的访问序列化；
QMutex的目的是保护对象，数据结构或代码段，以便一次只有一个线程可以访问它；  
    ```C++
        QMutex mutex;  
        int number = 6;  
        void method1()  
        {  
            mutex.lock();  
            number *= 5;  
            number /= 4;  
            mutex.unlock();  
        }
    ```

* 线程等待 
    ```C++
    thread1->start();
    thread1->wait();//程序将等待thread1完成后才执行后续
    qDebug()<<"hello world!";
    ```

* 线程与connect搭配使用[子线程通过信号控制控件]
  1、定义线程类
  2、线程类中定义信号，通过与ui中的槽函数绑定，来向ui传递数据
  3、ui中定义槽函数，用于接受数据并更新控件
  4、get
  * 线程类：
  ```C++
    //h文件mythread.h
    #ifndef MYTHREAD_H 
    #define MYTHREAD_H
    #include <QThread>
    class MyThread:public QThread
    {
        Q_OBJECT//需要用信号和槽的地方，都要添加这个宏，添加后可能会编译出错，执行一次清理和qmake即可
        public:
            MyThread();
            void run();
        private slots:

        private:
            int m_val;

        signals:
            void sig_setVal(int val);
    };
    #endif // MYTHREAD_H
  ```

  ```C++
    //cpp文件mythread.cpp
    #include "mythread.h"
    #include <QDebug>
    #include <QTime>
    #include <QtGlobal>

    MyThread::MyThread()
    {}

    void MyThread::run()
    {
        qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));
        int val = 0;
        for(int i=0;i<100;i++)
        {
            qDebug()<<"线程计算中";
            val = qrand()%100;
            emit sig_setVal(val);
            QThread::sleep(1);
        }
    }
  ```
    * ui中控件逻辑
  ```C++
    #include "mainwindow.h"
    #include "ui_mainwindow.h"
    #include <iostream>
    #include <QTime>
    #include <qdebug.h>
    #include "mythread.h"

    MainWindow::MainWindow(QWidget *parent) :
        QMainWindow(parent),
        ui(new Ui::MainWindow)
    {
        ui->setupUi(this);
        ui->lcdNumber->setAutoFillBackground(true);
    }

    MainWindow::~MainWindow()
    {
        delete ui;
    }

    void MainWindow::on_pushButton_clicked()//按钮【关闭】
    {
        this->close();
    }

    void MainWindow::on_pushButton_2_clicked()//按钮【随机】
    {
        MyThread *pmyth = new MyThread;
        connect(pmyth,SIGNAL(finished()),pmyth,SLOT(deleteLater()));//将线程结束事件与线程销毁函数绑定，实现自动delete
        connect(pmyth,SIGNAL(sig_setVal(int)),this,SLOT(db(int)));//将线程中的信号与ui中的槽函数绑定
        pmyth->start();
    }

    void MainWindow::db(int pvalue)//槽函数，用于在ui线程中更新控件
    {
        ui->progressBar1->setValue(pvalue);//控件1
        ui->lcdNumber->display(pvalue);//控件2
        ui->lineEdit1->setText(QString::number(pvalue));//控件3
        qDebug() << "控件数据已更新";
    }
```
