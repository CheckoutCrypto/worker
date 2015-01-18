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
#ifndef CONTROL_H
#define CONTROL_H

#include "mainthread.h"
#include "serverthread.h"
#include "src/server/server.h"
#include "src/db/cache.h"
#include "src/db/dataconnect.h"

#include "src/lib/menu.h"


class control : public QObject
{
    Q_OBJECT

public:
    control();
    virtual ~control();
    MainThread mThread;
    ServerThread sThread;

    Server serv;
    DataConnect dbconnect;
    cache cah;
    menu mnu;

    daeObj dae;
    dbObj db;
    grpobj grp;
    wrkObj wrk;

    string API;

    void main(string password);
    void initializeServer();
    string initCache(string mnPass);
    void printCache();

    int menuSelection();
    int DBMenuSelection();
    int DaeMenuSelection();


public slots:
    void startServ(){
        emit startServer();
    }
signals:
    /// server
    void startServer(void);
};

#endif // CONTROL_H
