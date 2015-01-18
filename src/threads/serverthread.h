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
#ifndef SERVERTHREAD_H
#define SERVERTHREAD_H

#include <QThread>
#include <QDebug>

using namespace std;
class ServerThread : public QThread
{
       Q_OBJECT
public:
    explicit ServerThread();
    virtual ~ServerThread();
       void run(void);

public slots:
       void MySlot( void ){
           exec();
       }
        void connection( void ){
            qDebug() << "server THREAD RUNNING" << endl;
        }
        void setApi(){
            qDebug() << "testing" << endl;
        }
        void startServ(){
            emit startServer();
        }

signals:
       void startServer( void );
};

#endif // SERVERTHREAD_H
