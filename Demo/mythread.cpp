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


int MyThread::getval()
{
    return this->m_val;
}

bool MyThread::setval(int val)
{
    this->m_val = val;
    return true;
}




