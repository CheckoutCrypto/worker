/* Copyright 2014-2015 Grant Hutchinson

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

One exception, this software is not allowed to be used by Simon Choucroun or any assosciate or affiliate individual/company
*/
#ifndef MAINTHREAD_H
#define MAINTHREAD_H

#include <QThread>
#include <QDebug>
#include <QCoreApplication>


class MainThread : public QThread
{
    Q_OBJECT
public:
    explicit MainThread();
    virtual ~MainThread();
    bool escape;

    void run(void){
        if(!escape){
           main_menu();
        }
    }
    bool main_menu();
    void servStart();
public slots:
    void connection(void){
        qDebug() << "NewConnection" << endl;
    }
signals:
    void startServer(void);
};
#endif // MAINTHREAD_H
