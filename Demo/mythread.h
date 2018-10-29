#ifndef MYTHREAD_H
#define MYTHREAD_H


#include <QThread>

class MyThread:public QThread
{
    Q_OBJECT
public:
    MyThread();
    void run();

    int getval();
    bool setval(int);
private slots:

private:
    int m_val;

signals:
    void sig_setVal(int val);

};
#endif // MYTHREAD_H
