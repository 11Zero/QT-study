#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>
#include <QTime>
#include<qdebug.h>
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
    ui->progressBar1->setValue(pvalue);
    ui->lcdNumber->display(pvalue);
    ui->lineEdit1->setText(QString::number(pvalue));
    qDebug() << "控件数据已更新";
}


