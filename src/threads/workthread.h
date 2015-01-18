#ifndef WORKTHREAD_H
#define WORKTHREAD_H

#include <QThread>
#include <QDebug>

class workthread: public QThread
{
       Q_OBJECT
public:

    explicit workthread();
    virtual ~workthread();

    void run(void);
};

#endif // WORKTHREAD_H
